#!/usr/bin/env python3
r"""
Build a SPIFFS image from the local `data` directory and upload it to an ESP8266.

Steps:
 1. Locate the Arduino15 directory automatically (Windows: C:\Users\<user>\AppData\Local\Arduino15).
 2. Find the newest mkspiffs tool version under packages/esp8266/tools/mkspiffs/*.
 3. Generate a SPIFFS .bin from the given data directory.
 4. Find the newest esp8266 core tools folder (hardware/esp8266/<version>/tools) and its upload.py.
 5. Call upload.py to flash the SPIFFS image at the specified flash address (default 0x200000).

Usage (example):
  python spiffs_flash.py --port COM7
Optional overrides:
  --data ./data --size 2076672 --page 256 --block 8192 --addr 0x200000
  --bin existing.spiffs.bin   (skip build, just upload)

On success prints the commands executed.
"""
import argparse
import os
import sys
import tempfile
import shutil
import subprocess
from pathlib import Path
from typing import Optional, List

try:
    from packaging.version import Version
except ImportError:
    # Fallback simple version parser if packaging isn't installed
    class Version:  # type: ignore
        def __init__(self, v: str):
            self._v = tuple(int(x) if x.isdigit() else x for x in v.replace('-', '.').split('.'))
        def __lt__(self, other: 'Version'):
            return self._v < other._v
        def __repr__(self):
            return f"Version({self._v})"


def find_latest_subdir(base: Path) -> Optional[Path]:
    if not base.exists():
        return None
    candidates = [p for p in base.iterdir() if p.is_dir()]
    if not candidates:
        return None
    def sort_key(p: Path):
        try:
            return Version(p.name)
        except Exception:
            return Version('0')
    candidates.sort(key=sort_key, reverse=True)
    return candidates[0]


def build_spiffs(mkspiffs_exe: Path, data_dir: Path, page_size: int, block_size: int, image_size: int, out_bin: Path) -> None:
    if not data_dir.exists():
        raise FileNotFoundError(f"Data directory not found: {data_dir}")
    cmd = [str(mkspiffs_exe), '-c', str(data_dir), '-p', str(page_size), '-b', str(block_size), '-s', str(image_size), str(out_bin)]
    print(f"[INFO] Running mkspiffs: {' '.join(cmd)}")
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0:
        print(res.stdout)
        print(res.stderr, file=sys.stderr)
        raise RuntimeError(f"mkspiffs failed with exit code {res.returncode}")
    print(f"[INFO] SPIFFS image created: {out_bin} ({out_bin.stat().st_size} bytes)")


def upload_image(upload_py: Path, port: str, address: int, image_bin: Path, erase_flash: bool = False, erase_region: bool = False, baud: int = 460800) -> None:
    if not image_bin.exists():
        raise FileNotFoundError(f"SPIFFS image not found: {image_bin}")
    
    # upload.py is just a wrapper - follows pattern: write_flash [options] address file
    # From manual example: python upload.py --port COM7 write_flash 0x200000 file.bin --end
    # Add --no-compress to avoid MD5 verification issues
    image_size = image_bin.stat().st_size
    
    # Build command - match the manual working example exactly
    # Manual: python upload.py --port COM7 write_flash 0x200000 file.bin --end
    if erase_flash:
        cmd = [sys.executable, str(upload_py),
               '--port', port, 'erase_flash', '--end', 
               '--port', port, 'write_flash', hex(address), str(image_bin), '--end']
    elif erase_region:
        cmd = [sys.executable, str(upload_py),
               '--port', port, 'erase_region', hex(address), hex(image_size), '--end',
               '--port', port, 'write_flash', hex(address), str(image_bin), '--end']
    else:
        # Simple write, exactly like manual example
        cmd = [sys.executable, str(upload_py),
               '--port', port, 'write_flash', hex(address), str(image_bin), '--end']
    
    print(f"[INFO] Running upload: {' '.join(cmd)}")
    res = subprocess.run(cmd, capture_output=True, text=True)
    print(res.stdout)
    if res.returncode != 0:
        print(res.stderr, file=sys.stderr)
        raise RuntimeError(f"Upload failed with exit code {res.returncode}")
    print("[INFO] Upload completed successfully.")


def main():
    parser = argparse.ArgumentParser(description="Build and upload SPIFFS image to ESP8266.")
    parser.add_argument('--port', required=True, help='Serial COM port, e.g. COM7')
    parser.add_argument('--data', default='data', help='Path to data directory (default: ./data)')
    parser.add_argument('--size', type=int, default=2076672, help='SPIFFS image size in bytes (default: 2076672)')
    parser.add_argument('--page', type=int, default=256, help='Page size (default: 256)')
    parser.add_argument('--block', type=int, default=8192, help='Block size (default: 8192)')
    parser.add_argument('--addr', type=lambda x: int(x, 0), default=0x200000, help='Flash write address (default: 0x200000)')
    parser.add_argument('--bin', help='Existing SPIFFS bin to upload (skip build)')
    parser.add_argument('--keep-temp', action='store_true', help='Keep temporary directory with generated image')
    parser.add_argument('--arduino-root', help='Override Arduino15 root directory path; if not set and default not found you will be prompted.')
    parser.add_argument('--erase', action='store_true', help='Erase entire flash before writing SPIFFS (WARNING: erases main firmware too)')
    parser.add_argument('--erase-region', action='store_true', help='Erase only the SPIFFS region before writing (safer, preserves main firmware)')
    parser.add_argument('--baud', type=int, default=460800, help='Upload baud rate (default: 460800)')
    args = parser.parse_args()

    def resolve_arduino_root() -> Path:
        if args.arduino_root:
            p = Path(args.arduino_root).expanduser()
            if not p.exists():
                print(f"[ERROR] Provided --arduino-root does not exist: {p}", file=sys.stderr)
                sys.exit(2)
            return p
        home = Path(os.path.expanduser('~'))
        default_path = home / 'AppData' / 'Local' / 'Arduino15'
        if default_path.exists():
            return default_path
        # Prompt user interactively
        print(f"[WARN] Default Arduino15 directory not found: {default_path}")
        while True:
            user_input = input("Enter full path to Arduino15 directory (or press Enter to abort): ").strip()
            if not user_input:
                print("[INFO] Aborted by user.")
                sys.exit(1)
            candidate = Path(user_input).expanduser()
            if candidate.exists():
                return candidate
            print(f"[WARN] Path does not exist: {candidate}")
    arduino_root = resolve_arduino_root()
    print(f"[INFO] Using Arduino15 root: {arduino_root}")

    mkspiffs_base = arduino_root / 'packages' / 'esp8266' / 'tools' / 'mkspiffs'
    mkspiffs_dir = find_latest_subdir(mkspiffs_base)
    if not mkspiffs_dir:
        raise FileNotFoundError(f"No mkspiffs version directories found under {mkspiffs_base}")
    mkspiffs_exe = mkspiffs_dir / ('mkspiffs.exe' if os.name == 'nt' else 'mkspiffs')
    if not mkspiffs_exe.exists():
        raise FileNotFoundError(f"mkspiffs executable not found: {mkspiffs_exe}")

    hardware_base = arduino_root / 'packages' / 'esp8266' / 'hardware' / 'esp8266'
    hw_version_dir = find_latest_subdir(hardware_base)
    if not hw_version_dir:
        raise FileNotFoundError(f"No esp8266 hardware versions under {hardware_base}")
    upload_py = hw_version_dir / 'tools' / 'upload.py'
    if not upload_py.exists():
        raise FileNotFoundError(f"upload.py not found: {upload_py}")

    if args.bin:
        image_path = Path(args.bin)
        print(f"[INFO] Using existing SPIFFS image: {image_path}")
    else:
        temp_dir = Path(tempfile.mkdtemp(prefix='spiffs_build_'))
        image_path = temp_dir / 'spiffs_image.bin'
        try:
            build_spiffs(mkspiffs_exe, Path(args.data), args.page, args.block, args.size, image_path)
        except Exception:
            if not args.keep_temp:
                shutil.rmtree(temp_dir, ignore_errors=True)
            raise
        if args.keep_temp:
            print(f"[INFO] Temporary build directory retained: {temp_dir}")
        else:
            print(f"[INFO] Temporary build directory: {temp_dir} (will be removed after upload)")

    upload_image(upload_py, args.port, args.addr, image_path, args.erase, args.erase_region, args.baud)

    if not args.bin and not args.keep_temp:
        # Remove when we created it and not retaining
        try:
            shutil.rmtree(image_path.parent, ignore_errors=True)
            print("[INFO] Cleaned up temporary build directory.")
        except Exception as e:
            print(f"[WARN] Failed to remove temporary directory: {e}")


if __name__ == '__main__':
    main()
