/* 
 * Arduino GPS clock with local time using NEO-6M module and 20x4 LCD.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
*/


#include <TinyGPS++.h>        // include TinyGPS++ library
#include <TimeLib.h>          // include Arduino time library
#include <SoftwareSerial.h>   // include software serial library
#include <LiquidCrystal.h>    // include LCD library

TinyGPSPlus gps;

#define S_RX    9   // define software serial RX pin
#define S_TX    8   // define software serial TX pin

SoftwareSerial SoftSerial(S_RX, S_TX);   // configure SoftSerial library

// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define time_offset   -14400  // define a clock offset of 3600 seconds (1 hour) ==> UTC + 1

// variable definitions
char Time[]  = "TIME: 00:00:00";
char Date[]  = "DATE: 00-00-2020";
byte last_second, Second, Minute, Hour, Day, Month;
int Year;

void setup(void)
{
  SoftSerial.begin(9600);  // initialize software serial at 9600 baud

  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);//

  //lcd.setCursor(1, 0); // move LCD cursor to column 1, row 0 [upper left position (0, 0)]
  //lcd.print("GPS CLOCK (UTC+1)");
}

void loop()
{
  while (SoftSerial.available() > 0)
  {
    if (gps.encode(SoftSerial.read()))
    {
      // get time from GPS module
      if (gps.time.isValid())
      {
        Minute = gps.time.minute();
        Second = gps.time.second();
        Hour   = gps.time.hour();
      }

      // get date drom GPS module
      if (gps.date.isValid())
      {
        Day   = gps.date.day();
        Month = gps.date.month();
        Year  = gps.date.year();
      }

      if(last_second != gps.time.second())  // if time has changed
      {
        last_second = gps.time.second();

        // set current UTC time
        setTime(Hour, Minute, Second, Day, Month, Year);
        // add the offset to get local time
        adjustTime(time_offset);

        // update time array
        Time[12] = second() / 10 + '0';
        Time[13] = second() % 10 + '0';
        Time[9]  = minute() / 10 + '0';
        Time[10] = minute() % 10 + '0';
        Time[6]  = hour()   / 10 + '0';
        Time[7]  = hour()   % 10 + '0';

        // update date array
        Date[14] = (year()  / 10) % 10 + '0';
        Date[15] =  year()  % 10 + '0';
        Date[9]  =  month() / 10 + '0';
        Date[10] =  month() % 10 + '0';
        Date[6]  =  day()   / 10 + '0';
        Date[7]  =  day()   % 10 + '0';

        // print time & date
        //print_wday(weekday());   // print day of the week
        lcd.setCursor(0, 0);     // move cursor to column 0 row 2
        lcd.print(Time);         // print time (HH:MM:SS)
        lcd.setCursor(0, 1);     // move cursor to column 0 row 3
        lcd.print(Date);         // print date (DD-MM-YYYY)

      }

    }

  }

}

// function for displaying day of the week
//void print_wday(byte wday)
//{
//  lcd.setCursor(5, 1);  // move cursor to column 5, row 1
//  switch(wday)
//  {
//    case 1:  lcd.print(" SUNDAY  ");   break;
//    case 2:  lcd.print(" MONDAY  ");   break;
//    case 3:  lcd.print(" TUESDAY ");   break;
//    case 4:  lcd.print("WEDNESDAY");   break;
//    case 5:  lcd.print("THURSDAY ");   break;
//    case 6:  lcd.print(" FRIDAY  ");   break;
//    default: lcd.print("SATURDAY ");
//  }

//}
// end of code.
