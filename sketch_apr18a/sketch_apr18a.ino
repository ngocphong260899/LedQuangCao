#include <Wire.h>
#include "RTClib.h"
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------The SPI, DMD, TimerOne and Font libraries are used.
#include <SPI.h>       
#include <DMD.h>    
#include <TimerOne.h>  
#include "SystemFont5x7.h"
char msg = "";


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------Configuration P10
#define DISPLAYS_ACROSS 1 //-> Number of P10 panels used, side to side.
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
//--------------------------------------------------------------------------------------

RTC_DS1307 rtc; //-> RTC Declaration

//--------------------------------------------------------------------------------------Variables for time and date
int _day, _month, _year, _hour24, _hour12, _minute, _second, _dtw;
int hr24;
String st;
char nameoftheday[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char month_name[12][12] = {"January","February", "March", "April", "May", "June", "Jult", "August", "September", "October", "November", "December"};
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------Variable for Millis
const long interval = 1000; //-> Retrieve time and date data every 1 second
unsigned long previousMillis = 0;
int setgio;
int setphut;
const long interval_for_date = 75; //-> For scroll speed
unsigned long previousMillis_for_date = 0;
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------Variable to display hours and minutes
char hr_24 [3];
String str_hr_24;
char mn [3];
String str_mn;
char sc [3];
String str_sc;
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
String strSecondRow;
char chrSecondRow[60];
int i=32;
int j;
int sr=1;
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------ScanDMD()
void ScanDMD() { 
  dmd.scanDisplayBySPI(); 
  
}
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------setup
void setup() {
  Serial.begin(9600);
  Serial.println("Arduino RTC DS1307");
  delay(1000);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled (Set the time and date based on your computer time and date)
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //-> Set the time and date based on your computer time and date. If that doesn't work, use this line of code outside of "if (! rtc.isrunning())"
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
   // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // following line sets the RTC to the date & time this sketch was compiled (Set the time and date based on your computer time and date)
// rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //-> Set the time and date based on your computer time and date. Use this line of code if it doesn't work in "if (! rtc.isrunning())"
  //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  // If the time and date are successfully set, then deactivate the code line (make the code a comment), then re-upload the code.
  // if not done then the time and date will return to the beginning when it was set when arduino is reset or restarted.

  Timer1.initialize(1000);          
  Timer1.attachInterrupt(ScanDMD);   
  dmd.clearScreen(true);  
  dmd.selectFont(SystemFont5x7);
}
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------loop
void loop() {
  //_____________________________________________________millis() to display time
 
  if(Serial.available()>0)
  {
    //msg = Serial.read();

    char message[20] = {};
   Serial.readBytesUntil('\0', message, 20) ;
   Serial.print(message);
   
   strSecondRow = message;
   strSecondRow.toCharArray(chrSecondRow,60);
   j=strlen(chrSecondRow)+(strlen(chrSecondRow)*5);
    
//    switch (msg)
//    {
//    case 'a':
//    strSecondRow = "fcdtd";
//    strSecondRow.toCharArray(chrSecondRow,60);
//    j=strlen(chrSecondRow)+(strlen(chrSecondRow)*5);
//    break;
// 
//    case 'b':
//    strSecondRow = "Di An Com";
//    strSecondRow.toCharArray(chrSecondRow,60);
//    j=strlen(chrSecondRow)+(strlen(chrSecondRow)*5);
//    break;       
//      }
        //strSecondRow = "Trung Anh";
     // strSecondRow.toCharArray(chrSecondRow,60);
    //  j=strlen(chrSecondRow)+(strlen(chrSecondRow)*5);
  }
 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; //-> save the last time

    GetDateTime(); //-> Retrieve time and date data from DS1307
    
    //=====================================================Showing hour in P10
    str_hr_24=String(_hour24);
    str_hr_24.toCharArray(hr_24,3);

    if (_hour24<10) {
      dmd.drawString(2, 0, "0", 1, GRAPHICS_NORMAL);
      dmd.drawString(8, 0, hr_24, 1, GRAPHICS_NORMAL);
    }
    else {
      dmd.drawString(2, 0, hr_24, 2, GRAPHICS_NORMAL);
    }
    //=====================================================
    
    //=====================================================Showing ":" in P10
    GetDateTime(); //-> Retrieve time and date data from DS1307
    if (_second %2 == 0) {
      dmd.drawString(14, 0, ":", 2, GRAPHICS_OR);
    }
    else {
      dmd.drawString(14, 0, ":", 2, GRAPHICS_NOR);
    }
    //=====================================================
    
    //=====================================================Showing minutes in P10
    str_mn=String(_minute);
    str_mn.toCharArray(mn,3);
    if (_hour24==22 & _minute==26 ) {
      strSecondRow = "Trung anh an";
      strSecondRow.toCharArray(chrSecondRow,60);
      j=strlen(chrSecondRow)+(strlen(chrSecondRow)*5);
      }



    if (_minute<10) {
      dmd.drawString(19, 0, "0", 1, GRAPHICS_NORMAL);
      dmd.drawString(25, 0, mn, 1, GRAPHICS_NORMAL);
    }
    else {
      dmd.drawString(19, 0, mn, 2, GRAPHICS_NORMAL);
    }
    //=====================================================
  }
  //_____________________________________________________

  //_____________________________________________________millis() for display / scrolling dates & other text
  unsigned long currentMillis_for_date = millis();
  if (currentMillis_for_date - previousMillis_for_date >= interval_for_date) {
    previousMillis_for_date = currentMillis_for_date; //-> save the last time 

    //=====================================================Holds date data and other text to display
   // switch (sr) {
    //case 1:
      //strSecondRow = String(nameoftheday[_dtw]) + ", " + String(_day) + "-" + String(month_name[_month-1]) + "-" + String(_year);
     // strSecondRow.toCharArray(chrSecondRow,60);
     // j=strlen(chrSecondRow)+(strlen(chrSecondRow)*5);
    // break;
   // case 2:
      //strSecondRow = "Trung Anh";
     // strSecondRow.toCharArray(chrSecondRow,60);
    //  j=strlen(chrSecondRow)+(strlen(chrSecondRow)*5);

      //break;
   // }
    //=====================================================
    
    i--;
    dmd.drawString(i, 9, chrSecondRow, strlen(chrSecondRow), GRAPHICS_NORMAL);
    if (i<=~j) {
      i=32;
      sr++;
      if (sr>2) sr=1;
    }
  }
  //_____________________________________________________
}
//--------------------------------------------------------------------------------------

//------------------------------------------------------------------------Subroutine to retrieve or update the time and date from DS1307
void GetDateTime() {
  DateTime now = rtc.now();
  _day=now.day();
  _month=now.month();
  _year=now.year();
  _hour24=now.hour();
  _minute=now.minute();
  _second=now.second();
  _dtw=now.dayOfTheWeek();

  hr24=_hour24;
  if (hr24>12) {
    _hour12=hr24-12;
  }
  else if (hr24==0) {
    _hour12=12;
  }
  else {
    _hour12=hr24;
  }

  if (hr24<12) {
    st="AM";
  }
  else {
    st="PM"; 
  }  
}
