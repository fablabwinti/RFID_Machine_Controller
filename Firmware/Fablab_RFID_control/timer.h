


Ticker tkSecond;                     // Second - Timer for Updating Datetime Structure


void ICACHE_RAM_ATTR Second_Tick()
{

  if (watchdog > 60)
  {
    Serial.println(F("PANIC! ERROR! FUBAR! ... Rebooting"));
    ESP.restart(); //something really, really bad happended (i.e. stuck in infinite loop), reboot
  }
  else watchdog++;

}


