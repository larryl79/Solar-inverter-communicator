void inverter_LCD_QPIGS_base();
void inverter_LCD_QPIGS_data();

/* ===================================================================== *
 *                                                                       *
 * Menu Callback Function                                                *
 *                                                                       *
 * ===================================================================== *
 *
 * EXAMPLE CODE:

// *********************************************************************
void your_function_name(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    //LCDML_UNUSED(param);
    // setup
    // is called only if it is started

    // starts a trigger event for the loop function every 100 milliseconds
    LCDML.FUNC_setLoopInterval(100);

    // uncomment this line when the menu should go back to the last called position
    // this could be a cursor position or the an active menu function
    // GBA means => go back advanced
    //LCDML.FUNC_setGBA() 

    //
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button or event status change

    // uncomment this line when the screensaver should not be called when this function is running
    // reset screensaver timer
    //LCDML.SCREEN_resetTimer();

    // check if any button is pressed (enter, up, down, left, right)
    if(LCDML.BT_checkAny()) {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
    // this function is always called when the functions ends.
    // this means when you are calling a jumpTo or a goRoot function
    // that this part is called before a function is closed
  }
}


 * ===================================================================== *
 */

// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);
error_code = 0;
    // setup function
    u8g2.setFont(_LCDML_DISP_font);
    u8g2.setFontMode(0);

    u8g2.firstPage();
    LCDML.FUNC_setLoopInterval(300);  // starts a trigger event for the loop function every 100 milliseconds
    
    do {
    inverter_LCD_QPIGS_base();  

      /* u8g2.drawStr( 2, (_LCDML_DISP_font_h * 1), "To close this");
      u8g2.drawStr( 2, (_LCDML_DISP_font_h * 2), "function press");
      u8g2.drawStr( 2, (_LCDML_DISP_font_h * 3), "any button or use");
      u8g2.drawStr( 2, (_LCDML_DISP_font_h * 4), "back button");
      */
    } while( u8g2.nextPage() );


  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
  String result="";
  error_code = inverter.receive(inverter.QPIGS, result );
  if (error_code == 0 ) 
    {
    
    inverter.store_QPIGS( result.c_str() , 0 );
    
    //lcd.QPIGS();
    }
    //inverter.console_data(inverter.QPIGS_values);
  do { inverter_LCD_QPIGS_data(); } while( u8g2.nextPage() );

    if(LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_func_timer_info = 0;  // time counter (global variable)
unsigned long g_timer_1 = 0;    // timer variable (global variable)
void mFunc_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    g_func_timer_info = 20;       // reset and set timer

    char buf[20];
    sprintf (buf, "wait %d seconds", g_func_timer_info);

    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), buf);
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "or press back button");
    } while( u8g2.nextPage() );



    LCDML.FUNC_setLoopInterval(300);  // starts a trigger event for the loop function every 100 milliseconds

    LCDML.TIMER_msReset(g_timer_1);
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // reset screensaver timer
    LCDML.SCREEN_resetTimer();

     // this function is called every 100 milliseconds

    // this method checks every 1000 milliseconds if it is called
    if(LCDML.TIMER_ms(g_timer_1, 1000))
    {
      g_timer_1 = millis();
      g_func_timer_info--;                // increment the value every second
      char buf[20];
      sprintf (buf, "wait %d seconds", g_func_timer_info);

      u8g2.setFont(_LCDML_DISP_font);
      u8g2.firstPage();
      do {
        u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), buf);
        u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "or press back button");
      } while( u8g2.nextPage() );

    }

    // this function can only be ended when quit button is pressed or the time is over
    // check if the function ends normally
    if (g_func_timer_info <= 0)
    {
      // leave this function
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}

// *********************************************************************
uint8_t g_button_value = 0; // button value counter (global variable)
void mFunc_p2(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    // print LCD content
    char buf[17];
    sprintf (buf, "count: %d of 3", 0);

    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "press a or w button");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), buf);
    } while( u8g2.nextPage() );

    // Reset Button Value
    g_button_value = 0;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML.BT_checkLeft() || LCDML.BT_checkUp()) // check if button left is pressed
      {
        LCDML.BT_resetLeft(); // reset the left button
        LCDML.BT_resetUp(); // reset the left button
        g_button_value++;

        // update LCD content
        char buf[20];
        sprintf (buf, "count: %d of 3", g_button_value);

        u8g2.setFont(_LCDML_DISP_font);
        u8g2.firstPage();
        do {
          u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "press a or w button");
          u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), buf);
        } while( u8g2.nextPage() );
      }
    }

   // check if button count is three
    if (g_button_value >= 3) {
      LCDML.FUNC_goBackToMenu();      // leave this function
    }
  }

  if(LCDML.FUNC_close())     // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "screensaver");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "press any key");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 3), "to leave it");
    } while( u8g2.nextPage() );

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())          // ****** STABLE END *********
  {
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);      // leave this function and go a layer back
  }
}


// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // go to root and display menu
    LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_jumpTo_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);
    
    // Jump to main screen
    LCDML.OTHER_jumpToFunc(mFunc_timer_info);
  }
}


// *********************************************************************
void mFunc_para(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {

    char buf[20];
    sprintf (buf, "parameter: %d", param);

    // setup function
    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), buf);
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "press any key");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 3), "to leave it");
    } while( u8g2.nextPage() );

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())               // ****** LOOP *********
  {
    // For example
    switch(param)
    {
      case 10:
        // do something
        break;

      case 20:
        // do something
        break;

      case 30:
        // do something
        break;

      default:
        // do nothing
        break;
    }


    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}

/* ===================================================================== *
 *                                                                       *
 * Conditions to show or hide a menu element on the display              *
 *                                                                       *
 * ===================================================================== *
 */

#include <Arduino.h>

// *********************************************************************
boolean COND_hide()  // hide a menu element
// *********************************************************************
{
  return false;  // hidden
}


void inverter_LCD_QPIGS_base()
{
  //int col = floor(_LCDML_DISP_max_w / _LCDML_DISP_font_w);
  
  // x,y
 // u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), buf);
  u8g2.setDrawColor(1);
  u8g2.drawStr((_LCDML_DISP_font_w * 0),(_LCDML_DISP_font_h * 1),  "Grid:");  // Abbreviation of Grid
  u8g2.drawStr((_LCDML_DISP_font_w * 0),(_LCDML_DISP_font_h * 2),  "Out :");  // Abbreviation of Output
  u8g2.drawStr((_LCDML_DISP_font_w * 0),(_LCDML_DISP_font_h * 3),  "Batt:");  // Abbreviation of Battery
  u8g2.drawStr((_LCDML_DISP_font_w * 0),(_LCDML_DISP_font_h * 4),  "PV   :");  // Abbreviation of PhotoVoltaic
  //print metrics
  u8g2.drawStr((_LCDML_DISP_font_w * 5),(_LCDML_DISP_font_h * 1),  "V");   // Grid Voltage
  u8g2.drawStr((_LCDML_DISP_font_w * 9),(_LCDML_DISP_font_h * 1),  "Hz");  // Grid frequency
  u8g2.drawStr((_LCDML_DISP_font_w * 14),(_LCDML_DISP_font_h * 1), "VA");  // Output load Apparent power (VA)

  u8g2.drawStr((_LCDML_DISP_font_w * 5),(_LCDML_DISP_font_h * 2),  "V");  //  Output Voltage
  u8g2.drawStr((_LCDML_DISP_font_w * 9),(_LCDML_DISP_font_h * 2),  "Hz"); //  Output Frequency
  u8g2.drawStr((_LCDML_DISP_font_w * 14),(_LCDML_DISP_font_h * 2), "W");  //  Active power (load) 
  
  u8g2.drawStr((_LCDML_DISP_font_w * 6),(_LCDML_DISP_font_h * 3),  "V");  // Battery Voltage
  u8g2.drawStr((_LCDML_DISP_font_w * 10),(_LCDML_DISP_font_h * 3), "A");  // Battery charge current
  u8g2.drawStr((_LCDML_DISP_font_w * 15),(_LCDML_DISP_font_h * 3), "%");  // Battery charge in %

  u8g2.drawStr((_LCDML_DISP_font_w * 6),(_LCDML_DISP_font_h * 4),  "V");  // PV voltage
  u8g2.drawStr((_LCDML_DISP_font_w * 10),(_LCDML_DISP_font_h * 4), "A");  // PV current
  u8g2.drawStr((_LCDML_DISP_font_w * 15),(_LCDML_DISP_font_h * 4), "W");  // PV power
  u8g2.drawStr((_LCDML_DISP_font_w * 18),(_LCDML_DISP_font_h * 4), "%");  // Inverter load in %  
}

char StrBuffer;

void inverter_LCD_QPIGS_data()
{
  //int col = floor(_LCDML_DISP_cols_max / _LCDML_DISP_font_w) ;
  int col = _LCDML_DISP_font_w ;
  
Serial.println(col);
  //u8g2.drawStr((_LCDML_DISP_font_w * 2), (_LCDML_DISP_font_h * 1),"UNIX TIME:................ " + String(inverter.QPIGS_values..unixtime) + " Epoch");

  
  u8g2.setDrawColor(0);
  
  u8g2.drawBox((_LCDML_DISP_font_w * 3 ), (_LCDML_DISP_font_h * 1 - _LCDML_DISP_font_h ), 11, _LCDML_DISP_font_h );  // gridVoltage
  u8g2.drawBox((_LCDML_DISP_font_w * 6 ), (_LCDML_DISP_font_h * 1 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // gridFrequency/10.0
  u8g2.drawBox((_LCDML_DISP_font_w * 3 ), (_LCDML_DISP_font_h * 2 - _LCDML_DISP_font_h ), 11, _LCDML_DISP_font_h );  // acOutput
  u8g2.drawBox((_LCDML_DISP_font_w * 6 ), (_LCDML_DISP_font_h * 2 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // acFrequency/10.0
  u8g2.drawBox((_LCDML_DISP_font_w * 11), (_LCDML_DISP_font_h * 1 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // acApparentPower
  u8g2.drawBox((_LCDML_DISP_font_w * 11), (_LCDML_DISP_font_h * 2 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // acActivePower
  u8g2.drawBox((_LCDML_DISP_font_w * 15), (_LCDML_DISP_font_h * 4 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // loadPercent

  u8g2.drawBox((_LCDML_DISP_font_w * 3 ), (_LCDML_DISP_font_h * 3 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // batteryVoltage/100.00
  u8g2.drawBox((_LCDML_DISP_font_w * 7 ), (_LCDML_DISP_font_h * 3 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // batteryChargeCurrent
  u8g2.drawBox((_LCDML_DISP_font_w * 11), (_LCDML_DISP_font_h * 3 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // batteryCharge

  u8g2.drawBox((_LCDML_DISP_font_w * 3 ), (_LCDML_DISP_font_h * 4 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // PVVoltage
  u8g2.drawBox((_LCDML_DISP_font_w * 7 ), (_LCDML_DISP_font_h * 4 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // PVCurrent
  u8g2.drawBox((_LCDML_DISP_font_w * 12), (_LCDML_DISP_font_h * 4 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // PVPower 
  // u8g2.drawBox((_LCDML_DISP_font_w * 15), (_LCDML_DISP_font_h * 4 - _LCDML_DISP_font_h ), 18, _LCDML_DISP_font_h );  // loadpercent

  u8g2.setDrawColor(1);
  
  dtostrf(inverter.QPIGS_values.gridVoltage,        4, 0, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 2.9 ), (_LCDML_DISP_font_h * 1), &StrBuffer ); // gridVoltage
  dtostrf(inverter.QPIGS_values.gridFrequency/10.0, 4, 1, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 6.5 ), (_LCDML_DISP_font_h * 1), &StrBuffer ); // gridFrequency/10.0
  dtostrf(inverter.QPIGS_values.acOutput,           4, 0, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 2.9 ), (_LCDML_DISP_font_h * 2), &StrBuffer ); // acOutput
  dtostrf(inverter.QPIGS_values.acFrequency/10.0,   4, 1, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 6.5 ), (_LCDML_DISP_font_h * 2), &StrBuffer ); // acFrequency/10.0
  dtostrf(inverter.QPIGS_values.acApparentPower,    4, 0, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 11), (_LCDML_DISP_font_h * 1), &StrBuffer ); // acApparentPower
  dtostrf(inverter.QPIGS_values.acActivePower,      4, 0, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 11), (_LCDML_DISP_font_h * 2), &StrBuffer ); // acActivePower
  dtostrf(inverter.QPIGS_values.loadPercent,        3, 0, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 16), (_LCDML_DISP_font_h * 4), &StrBuffer ); // loadPercent

  /*
  dtostrf(inverter.QPIGS_values.busVoltage/100.00,6,2, &StrBuffer);
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 8), (_LCDML_DISP_font_h * 4), &StrBuffer); 
  */
  dtostrf(inverter.QPIGS_values.batteryVoltage/100.00,       6, 2, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 2.5 ), (_LCDML_DISP_font_h * 3), &StrBuffer ); // batteryVoltage
  dtostrf(inverter.QPIGS_values.batteryChargeCurrent/100.00, 6, 1, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 7 ),   (_LCDML_DISP_font_h * 3), &StrBuffer ); // batteryChargeCurrent
  dtostrf(inverter.QPIGS_values.batteryCharge,               3, 0, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 12),   (_LCDML_DISP_font_h * 3), &StrBuffer ); // batteryCharge

  /*
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"PV_INVERTER Temperature:.. " + String(inverter.QPIGS_values.inverterTemperature /10.0) + " °C"); 
  */

  dtostrf(inverter.QPIGS_values.PVVoltage/10, 5, 1, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 3 ), (_LCDML_DISP_font_h * 4), &StrBuffer ); // PVVoltage
  dtostrf(inverter.QPIGS_values.PVCurrent/10, 6, 1, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 7 ), (_LCDML_DISP_font_h * 4), &StrBuffer ); // PVCurrent
  dtostrf(inverter.QPIGS_values.PVPower/100,  5, 1, &StrBuffer ); u8g2.drawStr((_LCDML_DISP_font_w * 12), (_LCDML_DISP_font_h * 4), &StrBuffer ); // PVPower 
  /*
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"Battery SCC:.............. " + String(inverter.QPIGS_values.batterySCC/100.00) + " V"); 
  */
  /*
  u8g2.drawStr("Batt DischargeCurrent:.... " + String(inverter.QPIGS_values..batteryDischargeCurrent) + " A"); 
  u8g2.drawStr("DeviceStatus:............. " + String(inverter.QPIGS_values..deviceStatus));
  u8g2.drawStr("DevStatus.SBUpriority:.... " + String(DevStatus.SBUpriority));
  u8g2.drawStr("DevStatus.ConfigStatus:... " + String(DevStatus.ConfigStatus));
  u8g2.drawStr("DevStatus.FwUpdate:....... " + String(DevStatus.FwUpdate));
  u8g2.drawStr("DevStatus.LoadStatus:..... " + String(DevStatus.LoadStatus));
  u8g2.drawStr("DevStatus.BattVoltSteady:. " + String(DevStatus.BattVoltSteady));
  u8g2.drawStr("DevStatus.Chargingstatus:. " + String(DevStatus.Chargingstatus));
  u8g2.drawStr("DevStatus.SCCcharget:..... " + String(DevStatus.SCCcharge));
  u8g2.drawStr("DevStatus.ACcharge:....... " + String(DevStatus.ACcharge)); 

  u8g2.drawStr("DeviceStatus bit 0:...... " + String(inverter.QPIGS_values.deviceStatus[0]));
  u8g2.drawStr("DeviceStatus bit 1:...... " + String(inverter.QPIGS_values.deviceStatus[1]));
  u8g2.drawStr("DeviceStatus bit 2:...... " + String(inverter.QPIGS_values.deviceStatus[2]));
  u8g2.drawStr("DeviceStatus bit 3:...... " + String(inverter.QPIGS_values.deviceStatus[3]));
  u8g2.drawStr("DeviceStatus bit 4:...... " + String(inverter.QPIGS_values.deviceStatus[4]));
  u8g2.drawStr("DeviceStatus bit 5:...... " + String(inverter.QPIGS_values.deviceStatus[5]));
  u8g2.drawStr("DeviceStatus bit 6:...... " + String(inverter.QPIGS_values.deviceStatus[6]));
  u8g2.drawStr("DeviceStatus bit 7:...... " + String(inverter.QPIGS_values.deviceStatus[7]));
  */
  if ( inverter.getProtocol() == 2 )   // 2 = 22 fields from QPIGS
  {
    u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),("Battery offset Fan:.... " + String(inverter.QPIGS_values.batOffsetFan) + " V").c_str());
    u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),("EEPROM Version:........ " + String(inverter.QPIGS_values.eepromVers)).c_str());
    u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),("PV1 Charger Power:..... " + String(inverter.QPIGS_values.PV1_chargPower) + " W").c_str());
    u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),("DeviceStatus2 bit 0:... " + String(inverter.QPIGS_values.deviceStatus2[0])).c_str());
    u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),("DeviceStatus2 bit 1:... " + String(inverter.QPIGS_values.deviceStatus2[1])).c_str());
    u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),("DeviceStatus2 bit 2:... " + String(inverter.QPIGS_values.deviceStatus2[2])).c_str());
  }
  // QPIRI values
  /*
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"Bat Back to Grid:........ " + String(inverter.QPIGS_values.bat_backToUtilityVolts/10.0) + " V"); 
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"Bat Bulk Charge:......... " + String(inverter.QPIGS_values.bat_bulkChargeVolts/10.0) + " V"); 
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"Bat Float Charge:........ " + String(inverter.QPIGS_values.bat_FloatChargeVolts/10.0) + " V"); 
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"Bat CutOff:.............. " + String(inverter.QPIGS_values.bat_CutOffVolts/10.0) + " V"); 
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"Output Priority:......... " + String(inverter.QPIGS_values.OutPutPriority) + " | 0: Utility first / 1: Solar first / 2: SBU first"); 
  u8g2.drawStr(2, (_LCDML_DISP_font_h * 1),"Charging Priority:....... " + String(inverter.QPIGS_values.ChargerSourcePriority) + " | 0: Utility first / 1: Solar first / 2: Solar + Utility / 3: Only solar"); 
*/
}