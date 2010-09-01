/*
 * Copyright 2010 Reef Angel / Roberto Imai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef	ReefAngel_h
#define ReefAngel_h

#define ReefAngel_Version "0.8.5 beta 6"
//#define DEV_MODE  // Developer Mode

/*
If your sketch/compile size is getting too big or if you are running out of RAM and having bizarre events
happen with the controller, you may want to not display the graphics on screen which can save some memory.
Just comment out the next line to prevent any graphics from being displayed, you will have text only screens
during water changes and feeding modes.

Approximately 346 bytes to have this feature
*/
#define DisplayImages  // do we display the graphics for feeding or water change mode

/*
The next line is for displaying the setup screens to configure the values for the Feeding Mode timer and
the LCD shutoff timer.  The defaults are fine, BUT if you would like to have the ability to change them
from the setup screen, uncomment the next line.  This will increase the file size.  If you do not plan
to change these values often (or at all), keep the next line commented out.

Approximately 362 bytes to have this feature
*/
//#define SetupExtras  // feeding mode & screensaver timeout setup. ACTIVATE WITH CAUTION

/*
Since we may or may not need to always configure the Wavemakers or Dosing Pumps, give the option to
turn off the setup screens to keep the compile size down.  You can still use the Dosing Pump or Wavemakers,
you just will not have the setup screens available to configure the values.  You will have to manually set
the intervals inside the Sketch (hardcode or have it read from memory if the memory contains the correct values).
Comment out the appropriate line to remove the corresponding setup screens.

Approximately 2000 bytes to have DosingPumpSetup
Approximately 378 bytes to have WavemakerSetup
*/
#define DosingPumpSetup
#define WavemakerSetup

/*
Overheat Temperature is fairly constant.  This value will most likely not get changed very often (if ever).
The default value is set to 150.0F.  Once this value is reached, all the lights will get shutoff.  If you
would like the ability to change this value from the menus, uncomment the next line.  Otherwise you will have
to hardcode the value in the ShowInterface Function

Approximately 156 bytes to have this feature
*/
//#define OverheatSetup

/*
The ability to set the Date & Time on the controller is controlled by this next line.  This line will add
in a Date / Time Setup menu entry which will allow you to set the date & time on the controller easily.
Comment the next line to remove this ability.

Approximately 1984 bytes to have this feature
*/
#define DateTimeSetup

/*
If you do not want to have a Version menu entry to see what version of the software is on the controller,
then you will want to comment out the next line

Approximately 144 bytes to have this feature
*/
#define VersionMenu

/*
If you do not use any of the ATO features in your setup, you can comment out this next line to remove
the ATO set and clear menu items.

Approximately 900 bytes to have this feature (and without SetupExtras)

When this or SetupExtras are defined, the Timeouts menu is included.
Timeouts menu requires approximately 710 bytes
This feature requires approximately 190 bytes if SetupExtras is defined
*/
#define ATOSetup

/*
If you do not use metal halides and do not wish to have any of the setup screens on your controller,
you can comment out the next line to remove the Metal Halide Setup and Metal Halide Delay

Approximately 258 bytes to have this feature
*/
#define MetalHalideSetup

/*
If you want to use the old way of reading the temp sensor which is always reading the value in
and not performing any sanity check, then you will want to uncomment this next line.

Otherwise, you will use the new way to handle the temperatures.  The value is read in and then
compared to the existing value.  If the difference between the 2 values is less than MAX_TEMP_SWING
or the temperature is 0 then the temperature is allowed to be updated, otherwise it is not updated.
MAX_TEMP_SWING is currently set to 50, which is 5.0 F.  This prevents any temporary large fluctations
in temperatures.  Also, there should not be more than a 5.0 F degree fluctation in 1 second.
*/
//#define DirectTempSensor

/*
Do we save the relay state before we enter Feeding Mode or Water Change Mode?

Comment out the next line to not save the state and always force specific ports to be turned off and then
back on again after we exit the mode.  This can turn on some ports that were not already on.  This
is also how the controller originally works.
*/
#define SaveRelayState

/*
If you have the wifi module for your controller and wish to use it, you must uncomment this next line
to utilize the built-in webserver for the controller.
*/
//#define wifi

#include <ReefAngel_EEPROM.h>  // NOTE read/write internal memory
#include <Time.h>
#include <ReefAngel_NokiaLCD.h>
#include <ReefAngel_ATO.h>
#include <ReefAngel_Joystick.h>
#include <ReefAngel_LED.h>
#include <ReefAngel_TempSensor.h>
#include <ReefAngel_Relay.h>
#include <ReefAngel_PWM.h>
#include <ReefAngel_Timer.h>
#include <ReefAngel_Memory.h>
#include <avr/pgmspace.h>


#ifdef wifi
#define SERVER_HEADER1 "HTTP/1.1 200 OK" CRLF "Server: ReefAngel" CRLF "Cache-Control: no-store, no-cache, must-revalidate" CRLF "Pragma: no-cache" CRLF "Connection: close" CRLF "Content-Type: text/"
#define SERVER_HEADER2 CRLF "Content-Length: "
#define SERVER_HEADER3 CRLF CRLF
#define SERVER_HEADER_HTML SERVER_HEADER1 "html" SERVER_HEADER2
#define SERVER_HEADER_XML SERVER_HEADER1 "xml" SERVER_HEADER2
#define SERVER_DEFAULT "<h1>Reef Angel Controller Web Server</h1>"
#define SERVER_RELAY ""
#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"
#define P(name)   static const prog_uchar name[] PROGMEM
//#define SERVER_MONITOR "<html><head><script type='text/javascript'>var t;\nfunction timedCount(){loadXMLDoc();t=setTimeout('timedCount()',1000);}\nfunction loadXMLDoc(){if (window.XMLHttpRequest){xmlhttp=new XMLHttpRequest();}else{xmlhttp=new ActiveXObject('Microsoft.XMLHTTP');}\nxmlhttp.onreadystatechange=function(){if (xmlhttp.readyState==4 && xmlhttp.status==200){x=xmlhttp.responseXML.documentElement;\n"
//#define SERVER_MONITOR1 "document.getElementById('T1').innerHTML=x.getElementsByTagName('T1')[0].text;document.getElementById('T2').innerHTML=x.getElementsByTagName('T2')[0].text;\ndocument.getElementById('T3').innerHTML=x.getElementsByTagName('T3')[0].text;document.getElementById('PH').innerHTML=x.getElementsByTagName('PH')[0].text;}}\nxmlhttp.open('GET','params',true);xmlhttp.send();}timedCount();\n</script></head><body>\n<p>Temp1: <div id='T1'></div></p><p>Temp2: <div id='T2'></div></p><p>Temp3: <div id='T3'></div></p><p>PH: <div id='PH'></div></p>\n</body></html>"
//#define SERVER_AJAX "<html><head><script type='text/javascript'>function t(){l();var t1=setTimeout('t()',1000);}function l(){if (window.XMLHttpRequest) {a=new XMLHttpRequest();}else{a=new ActiveXObject('Microsoft.XMLHTTP');}a.onreadystatechange=function(){if (a.readyState==4 && a.status==200){x=a.responseXML.documentElement;document.getElementById('T1').innerHTML=x.getElementsByTagName('T1').item(0).firstChild.data/10;document.getElementById('T2').innerHTML=x.getElementsByTagName('T2').item(0).firstChild.data/10;"
//#define SERVER_AJAX1 "document.getElementById('T3').innerHTML=x.getElementsByTagName('T3').item(0).firstChild.data/10;document.getElementById('PH').innerHTML=x.getElementsByTagName('PH').item(0).firstChild.data/100;}};a.open('GET','params',true);a.send(null);}t();</script><style type='text/css'>.style1 {font-family: Arial, Helvetica, sans-serif;font-weight: bold;font-size: 24px;position: relative;top: 35px;left:-23px;width: 150px;height: 149px;}</style>"
//#define SERVER_AJAX2 "</head><body><table border=0 cellspacing=0 cellpadding=0><tr><td colspan=4 align=center><img src='http://www.reefangel.com/images/ra.jpg'></td></tr><tr><td background='http://www.reefangel.com/images/t1.jpg' align=center><div id=T1 class=style1></div></td><td background='http://www.reefangel.com/images/t2.jpg' align=center><div id=T2 class=style1></div></td><td background='http://www.reefangel.com/images/t3.jpg' align=center><div id=T3 class=style1></div></td><td background='http://www.reefangel.com/images/ph.jpg' align=center><div id=PH class=style1></div></td></tr></table></body></html>"
//http://www.xul.fr/ajax-get-xml.html

const prog_uchar SERVER_RA[] PROGMEM = "<html><head><script language='javascript' src='http://www.reefangel.com/wifi/ra.js'></script><link href='http://www.reefangel.com/wifi/ra.css' rel='stylesheet' type='text/css'></head><body onLoad='t();'>"
"<table><tr><td colspan=4 align=center><img src='http://www.reefangel.com/images/ra.jpg'></td></tr><tr>"
"<td background='http://www.reefangel.com/images/t1.jpg' align=center><div id=T1 class=style1></div></td>"
"<td background='http://www.reefangel.com/images/t2.jpg' align=center><div id=T2 class=style1></div></td>"
"<td background='http://www.reefangel.com/images/t3.jpg' align=center><div id=T3 class=style1></div></td>"
"<td background='http://www.reefangel.com/images/ph.jpg' align=center><div id=PH class=style1></div></td>"
"</tr><tr><td height='184' colspan=4 align=center><table width='500'><tr><td height='184' background='http://www.reefangel.com/images/panel.jpg'><table width='400' class='style3'><tr class='style2'>"
"<td><div id='l1'></div></td><td><div id='l2'></div></td><td><div id='l3'></div></td><td><div id='l4'></div></td><td><div id='l5'></div></td><td><div id='l6'></div></td><td><div id='l7'></div></td><td><div id='l8'></div></td>"
"</tr><tr class='style2'>"
"<td><div id='s1'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s1Map'></div></td>"
"<td><div id='s2'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s2Map'></div></td>"
"<td><div id='s3'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s3Map'></div></td>"
"<td><div id='s4'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s4Map'></div></td>"
"<td><div id='s5'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s5Map'></div></td>"
"<td><div id='s6'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s6Map'></div></td>"
"<td><div id='s7'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s7Map'></div></td>"
"<td><div id='s8'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s8Map'></div></td>"
"</tr></table></td></tr><tr><td><div id='temp'><H1>Please Wait... Loading...</H1></div></td></tr></table></td></tr></table>"
"<map name='s1Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('11');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('10');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('12');\"></map>"
"<map name='s2Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('21');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('20');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('22');\"></map>"
"<map name='s3Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('31');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('30');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('32');\"></map>"
"<map name='s4Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('41');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('40');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('42');\"></map>"
"<map name='s5Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('51');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('50');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('52');\"></map>"
"<map name='s6Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('61');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('60');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('62');\"></map>"
"<map name='s7Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('71');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('70');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('72');\"></map>"
"<map name='s8Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('81');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('80');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('82');\"></map>"
"</body></html>";

static char m_pushback[32];
static char m_pushbackDepth=0;
static byte reqtype=0;
static unsigned long timeout;
static bool conn=false;
#endif

static unsigned long ButtonDebounce;
static unsigned long RAStart;


class ReefAngelClass
{

public:
	int PHMin,PHMax;
	ParamsStruct Params;
	ReefAngelClass();
	ReefAngel_NokiaLCD LCD;
	ReefAngel_JoystickClass Joystick;
	ReefAngel_LEDClass LED;
	DS1307RTC RTC;
	ReefAngel_ATOHighClass HighATO;
	ReefAngel_ATOLowClass LowATO;
	ReefAngel_TempSensorClass TempSensor;
	ReefAngel_RelayClass Relay;
#ifdef DisplayLEDPWM
	ReefAngel_PWMClass PWM;
#endif  // DisplayLEDPWM
	/*
	Timers:
	0 - Feeding Mode timer
	1 - Wavemaker 1 or Dosing Pump 1
	2 - Wavemaker 2 or Dosing Pump 2
	3 - Backlight timer / Sleep timer
	4 - Not used
	5 - Store params to eeprom
	*/
	ReefAngel_TimerClass Timer[6];
	// NOTE for nested menus
	byte SelectedMenuItem;

	void Init();
	void Refresh();
	void SetTemperatureUnit(byte unit);
	void StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute);
	void MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay);
	void StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp);
	void StandardFan(byte FanRelay, int LowTemp, int HighTemp);
	void StandardATO(byte ATORelay, int ATOTimeout);
	void SingleATO(bool bLow, byte ATORelay, byte byteTimeout, byte byteHrInterval = 0);
	void DosingPump(byte DPRelay, byte DPTimer, byte OnHour, byte OnMinute, int RunTime);
#ifdef VersionMenu
	void DisplayVersion();
#endif  // VersionMenu
	void ClearScreen(byte Color);

    // NOTE for nested menus
    void InitMenus();
    void ShowInterface();
    void DisplayMenu();
    void DisplayMenuHeading();
    void DisplayMenuEntry(char *text);
    void FeedingMode();
    void WaterChangeMode();
    void ProcessButtonPress();
    void ProcessButtonPressMain();
    void ProcessButtonPressSetup();
    void ProcessButtonPressLights();
    void ProcessButtonPressTemps();
#if defined SetupExtras || defined ATOSetup
    void ProcessButtonPressTimeouts();
#endif  // if defined SetupExtras || defined ATOSetup

    // NOTE Setup Screens
    bool SetupOption(int &v, int &y, int rangemin, int rangemax, byte maxdigits,
                     char* unit, char* subunit, char* title,
                     char* prefix1, char* prefix2);
    void SetupLightsOptionDisplay(bool bMetalHalide);
    void SetupCalibratePH();
#ifdef DateTimeSetup
    void SetupDateTime();
#endif  // DateTimeSetup
#ifdef DosingPumpSetup
    void SetupDosingPump();
#endif  // DosingPumpSetup

private:
	bool showmenu;
	time_t menutimeout;

	// NOTE for nested menus
	int menusptr[Total_Menus];
	byte menuqtysptr[Total_Menus];
	byte DisplayedMenu;
	byte PreviousMenu;
	bool redrawmenu;

};

#ifdef wifi
void WebResponse (const prog_uchar *response, long strsize);
void printP(const prog_uchar *str);
void PROGMEMprint(const prog_uchar str[]);
#endif


extern ReefAngelClass ReefAngel;  // make an instance for the user

#endif

