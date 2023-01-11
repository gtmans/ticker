/* ideetjes
 * indien geen nieuws kijk in gelderland of ugchelen
 */
#define debug      0
#define prowl      1            // send message to prowl
#define dosleep    1            // go to sleep i/o run the loop()
#define use_ticker 1            // use MAX72XX scrolling display
#define use_button 0
#define myversion  3            // Version  0 this option lets led stay on during deepsleep but buzzer works
                                // Version  1 this option led stays off during deepsleep but buzzer keeps buzzing (works fine without buzzer)    
                                // Version  2 this option does not use button (analog full is used) 
                                // Version  3 ledstrip and buzzer and button
#if myversion ==   3
  #define MyRGBled 0            // Neopixel strip is used
#else
  #define MyRGBled 1            // use of tricolor led
#endif

#include    "gewoon_secrets.h"  // or fill in your values below
//const     char* ssid     =    "myssid";        
//const     char* password =    "mypass";
//const     String KNMIkey =    "h45fdgj9jj";
//char      ProwlapiKey[51]=    "utpo5u098609fdgfhdsffgghypoihptykhkpyo22";
String      APELDOORN;
String      payload       =     "";
String      Link          =     "/feeds/city/apeldoorn.rss";
const char  *host         =     "alarmeringen.nl";
const String KNMI         =     "http://weerlive.nl/api/json-data-10min.php?key="+KNMIkey+"&locatie="+town;
//SHA1      fingerprint of certificate use web browser to view and copy into program
const char  fingerprint[] PROGMEM = "2F A5 F5 85 94 66 BC 89 25 E2 FA 21 D1 6F 5B B5 DB E4 55 F0";

#define nrsites 9
const char  *hosts[nrsites]= {"www.nu.nl"    ,"www.destentor.nl"  ,"www.ad.nl"         ,"www.rtlnieuws.nl","sportnieuws.nl"  ,"sportnieuws.nl","feeds.nos.nl"      ,"feeds.nos.nl"     ,"www.verkeerplaza.nl"};
String       Links[nrsites]= {"/rss/Algemeen","/apeldoorn/rss.xml","/apeldoorn/rss.xml","/rss.xml"        ,"/formule-1/feed/","/feed/"        ,"/nosnieuwsalgemeen","/nossportalgemeen","/rssfeed"};
String   StartWith[nrsites]= {"</title>"     ,"<item>"            ,"<item>"            ,"<item>"          ,"<item>"          ,"<item>"        ,"<item>"            ,"<item>"           ,"<item>"};
int          lines[nrsites]= {3              ,3                   ,3                   ,3                 ,6                 ,6               ,6                   ,6                  ,20};
const char   divider[]=      {'+'            ,'~'                 ,'~'                 ,'+'               ,'?'               ,'?'             ,'!'                 ,'!'                ,'?'};
//int        Ports[nrsites]= {443,443,443,433,433,433,433,433};
const int    httpsPort= 443;  //HTTPS= 443 and HTTP = 80
const char   fingerprint0[]  PROGMEM="E7 F4 1F E5 E1 18 0E 41 62 F0 9E 42 E3 E1 2B 9D 51 2C 89 29";
const char   fingerprint1[]  PROGMEM="69 26 EE 96 86 5E 06 CD 47 C0 77 6A 72 47 CD E5 7D 79 C0 EE";
const char   fingerprint2[]  PROGMEM="D4 87 2A 58 83 36 83 54 D7 5B FD E8 04 75 F6 0A C5 A6 07 2D";
const char   fingerprint3[]  PROGMEM="12 7E AD C1 A4 5D 0D AF 0E 37 0E 40 A5 71 72 29 45 A5 B8 68";
const char   fingerprint4[]  PROGMEM="2D E3 7D D9 0E 4E 46 E6 60 9B 40 8A 34 26 FA 3F 50 77 7A AE";
const char   fingerprint5[]  PROGMEM="2D E3 7D D9 0E 4E 46 E6 60 9B 40 8A 34 26 FA 3F 50 77 7A AE";
const char   fingerprint6[]  PROGMEM="85 8E DF 39 3E BF 93 B3 7F 02 D8 A2 0C 4A 86 BD 69 6D D5 26";
const char   fingerprint7[]  PROGMEM="85 8E DF 39 3E BF 93 B3 7F 02 D8 A2 0C 4A 86 BD 69 6D D5 26";
const char   fingerprint8[]  PROGMEM="A8 9E E4 F7 E6 91 03 19 83 67 11 25 4C 60 88 B9 7E 91 EC 3F";
String       ShortHost;

//bool         debug         =     false;
bool         use_buzzer    =     true;
bool         urgent        =     false;
bool         new_urgent    =     false;
bool         forget        =     false;
bool         flop;
bool         morning;
bool         DST;
bool         LEAP          =     false;
bool         failed;
int          use_led       =     0;   
int          anaread;                                  //analogread value          
int          pos[12];
int          count;
int          site;
int          sitenr;
int          allcount;
byte         eepcount;
byte         sitcount;
int          skipplus      =     6;                    // length of " +0000"
int          titltype          [6];                    // 112  P2000 brandweer "P 1 Bon" "P 2 Bon" ambulance "Ambu A1 A2" trauma politie
String       descr;
String       title;
String       pubdt;
String       message;
String       MessageType;
String       RepStr;
String       lastBuildDate      = "<lastBuildDate>";
String       HTML_description   = "<description>";
String       HTML_pubdate       = "<pubDate>";
String       HTML_title         = "<title>";
String       HTML_item          = "<item>";
String       creation;
String       last_creation;
String       pubdate;
String       last_pub;
String       titl [12];                                // "Buitenbrand (container) op Laan van de Charleston in Apeldoorn"
String       tijd [12];                                // "09:41"
String       desc [12];                                // "p 1 bon-07 ass. ambu. afhijsen schrijnwerkershorst apeldoorn 062651 062631"
String       scrol[12];                                // message on scroll
int          mtype[12];                                // yellow=ambu blue=politie red=brandweer
int          prio;
String       yearStamp;
String       monthStamp;
String       daysStamp;
String       dateStamp;
String       TimeStamp;
String       formattedDate;
int          RThour;
int          Tmonth;
int          Tyear;
int          daynr;
int          mon;
int          DOW;                                      // Day Of Week 0=sunday 6=saturday
int          offset=6;                                 // 202200101 -> 6
int          checkyear;
int          StartDST;
int          EndDST;
String       allmonths     = "JanFebMarAprMayJunJulAugSepOctNovDec";
int          monthdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
String       names[14]     = {"red","orange","yellow","apple","green","turqouise","cyan","ocean","blue","violet","magenta","raspberry","white","uit"};
int          colors[36+6]  = {255,0,0,255,125,0,255,255,0,125,255,0,0,255,0,0,255,125,0,255,255,0,125,255,0,255,255,125,0,255,255,0,255,255,0,125,255,255,255,0,0,0};
int          today=0;
byte         Thour;
byte         old_Thour;
int          Tday;
String       found;
String       WTXT="";
String       WTXT2="";

#if MyRGBled
  #define uit     99  // not a pin
  #define yellow  100 // not a pin
  #define cyan    101 // not a pin
  #define magenta 102 // not a pin
  #define white   103
#else
  #include <Adafruit_NeoPixel.h>
  #define uit     13  // not a pin but index in array colors[]
  #define yellow  2   // not a pin but index in array colors[]
  #define cyan    6   // not a pin but index in array colors[]
  #define magenta 10  // not a pin but index in array colors[]
  #define white   12  // not a pin but index in array colors[]
#endif

#if dosleep 
  #include        <EEPROM.h>
  #define         uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds
  int sleepmins = 1;                      //#minutes 2 sleep 70max;
  int sleeptime = sleepmins * 60; 
  unsigned long   sleepingtime  = (sleeptime * uS_TO_S_FACTOR); 
  #define eepsize 512
  #ifdef ESP8266
    String  readword;
    byte    readbyte;
    int     address;
    int     loc;
    int     slen;
  #endif
#endif

#if prowl 
  #include <EspProwl.h>
  //char ProwlapiKey[51] = "jtffkwk5ej65v4kj67v57kjv8k7v8658jk678j99";  
#endif

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>           // https://github.com/taranais/NTPClient
#include <WiFiUdp.h>
WiFiUDP ntpUDP;                  // Define NTP Client to get time
NTPClient timeClient(ntpUDP);    // Variables to save date and time
  
#define     A_port        A0       //serial
#define     lo_button     D0       //GPIO16 IO   
//  v1.0 this option lets led stay on during deepsleep but buzzer works
#if myversion==0
  #define   button        D1 //GPIO5  IO, SCL
  #define   buzzer        D2 //GPIO4  IO, SDA
  #define   red           D3 //GPIO0  IO, 10k Pull-up
  #define   blue          D4 //GPIO2  IO, 10k pull-up, BUILTIN_LED
  #define   green         D8 //GPIO15 IO,10k pull-down, SS
#endif
//  v1.1 this option led stays off during deepsleep but buzzer keeps buzzing (works fine without buzzer)    
#if myversion==1
  #define   red           D1 //GPIO0  IO, 10k Pull-up
  #define   blue          D2 //GPIO2  IO, 10k pull-up, BUILTIN_LED
  #define   button        D3 //GPIO5  IO, SCL
  #define   buzzer        D4 //GPIO4  IO, SDA
  #define   green         D8 //GPIO15 IO,10k pull-down, SS
#endif
//  v1.2 this option does not use button (analog full is used)    
#if myversion==2
  #define   red           D1 //GPIO0  IO, 10k Pull-up
  #define   blue          D2 //GPIO2  IO, 10k pull-up, BUILTIN_LED
  #define   buzzer        D4 //GPIO4  IO, SDA
  #define   green         D8 //GPIO15 IO,10k pull-down, SS
#endif
//  v1.3 ledstrip and buzzer and button
#if myversion==3
  #define   red           0
  #define   blue          8
  #define   green         4
  #define   neopixels     2   // nr pixels
  #define   buzzer        D2
  #define   neopin        D1
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(neopixels, neopin, NEO_GRB + NEO_KHZ800);    
#endif
//other pins
#define   CLK_PIN       D5 //GPIO14 IO, SCK
#define   CS_PIN        D6 //GPIO12 IO, MISO
#define   DATA_PIN      D7 //GPIO13 IO, MOSI
//scrolling messages
#if use_ticker
  #include <MD_Parola.h>
  #include <MD_MAX72xx.h>
  #include <SPI.h>
  #define  HARDWARE_TYPE MD_MAX72XX::FC16_HW
  #define  MAX_DEVICES 8
  char*    tekst[5];
  int      nr      = 0;
  int      cnt     = 0;
  int      flip;
  int      flipval = 75;
  bool     flipon  = false; 
  MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
#endif

//############################### SETUP ############################## 
void setup() {
  Serial.begin      (115200);
  while(!Serial);   // time to get serial running
  delay             (1000);
  Serial.println    ();
  Serial.println    (__FILE__);//name of this doc
  pinMode           (buzzer,OUTPUT);
  #if use_button
    pinMode         (button,INPUT );
    if (digitalRead (button)==HIGH){Serial.println("button HIGH");}else{Serial.println("button LOW");}
  #endif

  anaread   =   analogRead           (A_port);
  Serial.print                       ("Analog "+String(anaread)+" ");
  if  (anaread>1000)                 {use_led=3  ;Serial.println("forget mode");forget=true;eepcount=0;sitcount=0;}//collect weather next time no news
  if  (anaread> 666&&anaread<=1000)  {use_led=2  ;Serial.println("leds all");} 
  if  (anaread> 333&&anaread<=666 )  {use_led=1  ;Serial.println("leds only latest");} 
  if  (anaread<=333)                 {use_led=0  ;Serial.println("leds off");}
  
  #if MyRGBled
      pinMode       (red,   OUTPUT);
      pinMode       (blue,  OUTPUT);
      pinMode       (green, OUTPUT);
      if (debug)    {SetMyLed(red);delay(1000);SetMyLed(blue);delay(1000);SetMyLed(yellow);delay(1000);SetMyLed(uit);}
  #else
  //  pinMode       (neopin, OUTPUT); 
      pixels.setBrightness(8);
      pixels.begin  ();
      if (use_led>0){for(int i=0;i<2;i++){BlinkPixels("green");delay(250);}}
      BlinkPixels   ("uit");
  #endif 
   
  Connect2WIFI(); 
  #if dosleep 
    readfromEeprom();
  #endif
    
  #if prowl
    EspProwl.begin                ();
    EspProwl.setApiKey            (ProwlapiKey);
    EspProwl.setApplicationName   ("gewoonthuis");
  #endif

  //TIME RTC and DST
  //RTC Initialize a NTPClient to get time
  timeClient.begin                (); 
  timeClient.setTimeOffset        (3600);  
  //TIME
  while(!timeClient.update())     {timeClient.forceUpdate();}
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate   = timeClient.getFormattedDate();
  pos[0]          = formattedDate.indexOf("T");
  dateStamp       = formattedDate.substring(0, pos[0]);
  TimeStamp       = formattedDate.substring(pos[0]+1, formattedDate.length()-1);  //16:56:20
  RThour          = TimeStamp.substring(0,2).toInt();
  yearStamp       = dateStamp.substring(0,4);
  monthStamp      = dateStamp.substring(5,7);
  daysStamp       = dateStamp.substring(8,10);
  Tyear           = yearStamp.toInt();
  Tmonth          = monthStamp.toInt();
  Tday            = daysStamp.toInt();  
  if (debug){
    Serial.println  ("formattedDate:"+formattedDate);
    Serial.println  ("dateStamp    :"+dateStamp);
    Serial.println  ("RThour       :"+String(RThour));
    Serial.println  ("TimeStamp    :"+TimeStamp);
    Serial.println  ("Tyear        :"+String(Tyear));
    Serial.println  ("Tmonth       :"+String(Tmonth));
    Serial.println  ("Tday         :"+String(Tday ));    
  }
  
  getDST(Tyear);
  if (DST){
    timeClient.setTimeOffset(7200);  
    while(!timeClient.update()) {timeClient.forceUpdate();}
    RThour++;
    if (debug){Serial.println("RThour       :"+RThour);}
  }
  if (Thour==0){Thour=RThour;old_Thour=RThour;}   
} // end of setup

//############################### LOOP ############################### 
void loop() {  
  getWebData              (true);               //check webdata only get 1st lines 2 update pubdate & creation
  if (creation!=last_creation){                 //Something happened
    Serial.println        ("Something happened. Lets check it out!");
    getWebData            (false);              //get webdata 1st 10 lines
    ParseData();                                //parse webdata
    last_creation   =     creation;             //reset vars    
    last_pub        =     pubdate; 
//  if (Thour<old_Thour)  {eepcount=0;Serial.println("its a new day!");for(int i=0;i<9;i++){BlinkPixels("ocean");delay(250);}}    // its a new day!  pa 4:12 vs 23:23
    if (new_urgent)       {TakeAction();}       //take action: show 1 message and prowl    
    else                  {DisplayMessages();}  //display max 10 messages acccording to analogread  
    #if dosleep 
      Write2Eeprom();
    #endif  
  } else {              // NOTHING CHANGED
      if (use_led>1)     {SetMyLed(red);}     
      Serial.print       ("No change since        :");
      Serial.println     (creation);
      Serial.print       ("latest pubdate         :");
      Serial.println     (pubdate);

      if (eepcount==0)  {                           //get & display weather
        Thour       =    old_Thour;//PRESERVE tHOUR VALUE
        getWeatherData  ();
        if (debug){
          Serial.println  ("getWeatherData()");
          Serial.println  ("WTXT en WTXT2");
          Serial.println  (WTXT);
          Serial.println  (WTXT2);
        }
        Write2Eeprom    ();   // save WTXT       
        DisplayWeather  (2);  // and display WTXT2 (alternative)     
      }
/*
      if (eepcount==2 ||eepcount==4 ||eepcount==6 )  {GetSomeNews(sitcount);sitcount++;} //get & display news
      if (eepcount==8)                               {DisplayWeather(1);}                //display weather
      if (eepcount==10||eepcount==12||eepcount==14)  {GetSomeNews(sitcount);sitcount++;} //get & display news
      if (sitcount>=nrsites)                         {sitcount=0;}
      eepcount++;
      if (eepcount>=14)                              {eepcount=0;}                       //display and start over
*/
      if (eepcount==3 ||eepcount==6 ||eepcount==9 )  {GetSomeNews(sitcount);sitcount++;} //get & display news
      if (eepcount==12)                              {DisplayWeather(1);}                //display weather
      if (eepcount==15||eepcount==18||eepcount==21)  {GetSomeNews(sitcount);sitcount++;} //get & display news
      if (sitcount>=nrsites)                         {sitcount=0;}
      eepcount++;
      if (eepcount>=21)                              {eepcount=0;}                       //display and start over

      // save eepcount && sitcount allways in eeprom
      #if dosleep 
        EEPROM.begin       (eepsize); 
        address = 0;  
        writeByte          (address,eepcount);
        Serial.println     ("Writing eepcount       :"+String(eepcount));
        writeByte          (address,sitcount);
        Serial.println     ("Writing sitcount       :"+String(sitcount));
      #endif
            
      #if use_ticker
        if (use_led>1){
          myDisplay.begin();
          myDisplay.setIntensity  (0);// Set the intensity (brightness) of the display (0-15):
          myDisplay.displayClear  ();
          myDisplay.displayText   ("no news", PA_CENTER, 100, 0, PA_OPENING, PA_CLOSING);
          while                   (!myDisplay.displayAnimate()){delay(5);}
          myDisplay.displayReset  ();  
        }      
      #endif
      
    } // end of change/nochange

    if (debug){
      Serial.println     ("Thour    ="+String(Thour));
      Serial.println     ("old_Thour="+String(old_Thour));
      Serial.println     ("eepcount ="+String(eepcount));
      Serial.println     ("sitcount ="+String(sitcount));      
    }
    
    #if dosleep 
      ByeBye                    ();       // sleep a minute
    #else                                 // blink a led
      for     (count=0;count<20;count++){
        if    (use_led>1){SetMyLed(green);}
        delay (1500);
        if    (use_led>1){SetMyLed(uit);}
        delay (1500);
      }
    #endif

}

//############################### END OF LOOP ############################### 

void Connect2WIFI(){    
  WiFi.mode (WIFI_OFF);     //Prevents reconnection issue (taking too long to connect)
  delay     (1000);
  WiFi.mode (WIFI_STA);     //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  WiFi.begin                (ssid, password);
  Serial.print              ("Connecting ");
  int t=0;
  while (WiFi.status()    != WL_CONNECTED) {
    t++;
    if (t>25){if (use_led>0){SetMyLed(uit);SetMyLed(red);delay(500);ESP.restart();}}
    Serial.print          (".");
    if (use_led>0){
      if (MyRGBled)       {SetMyLed(green);}
      else                {BlinkPixels("green");}
    }
    delay                 (250);
    if   (use_led>0){
      if (MyRGBled)       {SetMyLed(uit);}
      else                {BlinkPixels("green");}
    }
    delay                 (250);
  } 
  if (!MyRGBled)          {BlinkPixels("uit");}
  Serial.print            ("Connected to ");
  Serial.print            (ssid);
  Serial.print            ("with IP Address: ");
  Serial.println          (WiFi.localIP())  ;
}

void DisplayMessages(){
  anaread     =           analogRead(A_port);
  if   (anaread>1000)     {anaread-=100;}
  int   a     =           anaread/100;
  Serial.println          ("Serial value="+String(anaread)+" a="+String(a));
  if  (a>9||a<0)          {Serial.println("Something went wrong a="+String(a));a=9;}
  if  (a>count)           {Serial.println ("max messages="+String(count));a=count;}
    for (int s=0;s<=a;s++){
      flip=0;
      Serial.println            ("displaying message      :"+ String(s+1)+" from "+String(a+1)+":"+scrol[s]);      
      if (debug)                {Serial.println("Blinking "+names[mtype[s]]);}
      #if use_ticker
      if (!new_urgent){               // display not initialised yet
        myDisplay.begin();
        myDisplay.setIntensity  (0);  // Set the intensity (brightness) of the display (0-15):
        myDisplay.displayClear  ();
      }
      message = scrol[s];
      char    P5 [message.length()+1];
      message.toCharArray       (P5, message.length()+1);
      myDisplay.displayText     (P5, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      for (nr=0;nr<50000;nr++)  { 
        if    (myDisplay.displayAnimate()){myDisplay.displayReset();break;}
        flip++;
        if  (flip==flipval)     {BlinkPixels(names[mtype[s]]);}
        if  (flip>=2*flipval)   {BlinkPixels(names[mtype[s]]);flip=0;}                                                                                                                                                                                                                                                                     
        delay(5);
       }
       if (!MyRGBled)           {BlinkPixels("uit");}
    #endif  
    }
}

void TakeAction(){              //take action
  GetMsgType(mtype[0]);         //pa red-> MessageType="Brandweer"
  char    P0                    [MessageType.length()+1];
  MessageType.toCharArray       (P0, MessageType.length()+1);  
  // send prowl message
  if (prowl) { // send latest message if it is urgent
    if (use_led>1)              {SetMyLed(magenta);}
    message = scrol[0];
    char    P2                  [message.length()+1];
    message.toCharArray         (P2, message.length()+1);
    EspProwl.push               (P0,P2,0);
    if (use_led>1)              {delay(500);SetMyLed(uit);}     
  }// end of prowl
  #if use_ticker                      //init display
    myDisplay.begin();
    myDisplay.setIntensity      (0);// Set the intensity (brightness) of the display (0-15):
    myDisplay.displayClear      ();
    //display category
    myDisplay.displayText       (P0,PA_CENTER, 100, 0, PA_OPENING, PA_CLOSING);
  #endif    
  if (debug)                    {Serial.println("Blinking "+names[mtype[0]]);}
  if (use_buzzer){
    flip=0;tone(buzzer, 500);
    #if MyRGBled
      while (!myDisplay.displayAnimate()){flip++;if(flip==flipval)  {tone(buzzer,375);if (use_led>1){SetMyLed(mtype[0]);}}else{delay(5);}if(flip==2*flipval){tone(buzzer,500);if (use_led>1){SetMyLed(uit);}flip=0;}}
    #else
      while (!myDisplay.displayAnimate()){flip++;if(flip==flipval  ){tone(buzzer,375);BlinkPixels(names[mtype[0]]);}else{delay(5);}
                                                 if(flip==2*flipval){tone(buzzer,500);BlinkPixels(names[mtype[0]]);flip=0;}
                                         }
    #endif
    noTone(buzzer);
  } else {while                 (!myDisplay.displayAnimate()){delay(5);}}
  message = scrol[0];
  char    P6 [message.length()+1];
  message.toCharArray           (P6, message.length()+1);
  myDisplay.displayText         (P6, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  if (debug){Serial.println     ("Blinking "+names[mtype[0]]);}
  Serial.println                ("displaying message      :"+ scrol[0]);      

  for (nr=0;nr<50000;nr++)  { 
    if    (myDisplay.displayAnimate()){myDisplay.displayReset();break;}
    flip++;
    if  (flip==flipval)   {BlinkPixels(names[mtype[0]]);}
    if  (flip>=2*flipval) {BlinkPixels(names[mtype[0]]);flip=0;}                                                                                                                                                                                                                                                                     
    delay(5);
   }
  if (use_led>1)          {SetMyLed(uit);BlinkPixels("uit");}
  if (!MyRGBled)          {BlinkPixels("uit");}
  noTone(buzzer);
  myDisplay.displayReset    ();
  Serial.println            ("ready");    
}//end of void tAKEACTION

#if use_ticker
void DisplayWeather(int type){
  if (type==1){
     message = WTXT; 
  } else {
     message = WTXT2;
  }
  Serial.println          ("Now displaying         :"+message);
  char   W0 [message.length()+1];
  message.toCharArray     (W0, message.length()+1);
  myDisplay.begin();
  myDisplay.setIntensity  (0);// Set the intensity (brightness) of the display (0-15):
  myDisplay.displayClear  ();
  myDisplay.displayText   (W0, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!myDisplay.displayAnimate()){delay(5);}
  myDisplay.displayReset();  
}
#else
  Serial.println  (WTXT);  
#endif

/*Sample data
  <lastBuildDate>Sat, 08 Oct 2022 13:13:22 +0000</lastBuildDate>
  <item><title>a2 ambu 06127 dia apeldoorn rit 100481</title>
  <description>Ambulance met gepaste spoed naar Apeldoorn</description>
  <pubDate>Thu, 06 Oct 2022 11:23:20 +0000</pubDate>
*/
void ParseData(){
    if (use_led>1){SetMyLed(cyan);}

//  pos[1] is start of "</lastBuildDate>"
    pos[7]      =   pos[1]+lastBuildDate.length()+1;
    pos[2]      =   payload.indexOf("<title>" ,pos[7]);
    count       =   0;
    allcount    =   0;
do {
    pos[3] = payload.indexOf("</title>"      ,pos[2]);
    pos[4] = payload.indexOf("<description>" ,pos[3]);
    pos[5] = payload.indexOf("</description>",pos[4]);
    pos[6] = payload.indexOf("<pubDate>"     ,pos[5]);
    pos[7] = payload.indexOf("</pubDate>"    ,pos[6]);
    
    descr = payload.substring(pos[4]+HTML_description.length(),pos[5]);//<description>descr</description>
    title = payload.substring(pos[2]+HTML_title.length()      ,pos[3]);//<title>title</title>
    pubdt = payload.substring(pos[6]+HTML_pubdate.length()    ,pos[7]);//pubDate "Thu, 06 Oct 2022 11:23:20 +0000"


    //<title>urgent: a2 b1 b2 "p 2"
    prio=0;
    titl[count]   = title;
    if              (title.indexOf("a2")==0||title.indexOf("b2")==0||title.indexOf("b1")==0)
                    {urgent=false;}else{urgent=true;if(allcount==0){new_urgent=true;}}   
    if              (title.indexOf("p 3")!=-1||title.indexOf("p3")!=-1){prio=3;}
    if              (title.indexOf("p 2")!=-1||title.indexOf("p2")!=-1){prio=2;}
    if              (title.indexOf("p 1")!=-1||title.indexOf("p1")!=-1){prio=1;}
                    
    //<pubdate> 
    //compensate DST
    //<pubDate>Thu, 06 Oct 2022 11:23:20 +0000</pubDate>
    int    p      = pubdt.indexOf(":");
    String sy     = pubdt.substring(p-7,p-3);           // 2022
    int    y      = sy.toInt();
    String sm     = pubdt.substring(p-7-4,p-3-5);       // oct
    int    m      = (allmonths.indexOf(sm)/3)+1;    
    String sd     = pubdt.substring(p-7-4-3,p-3-5-4);   // oct
    int    d      = sd.toInt();  
    String hr     = pubdt.substring(p-2,p);             // pubDate hour "11"
    int    h      = hr.toInt(); 
    Tmonth        = m;
    Tday          = d;
    if(allcount==0){
      Thour         = h;  //int.to_bytes()
      if (debug)      {Serial.println("hr  :"+hr+"<");Serial.println("h   :"+String(h)+"<");} 
      if (DST){h+=2;}  else {h+=1;}
      if (h>=24)      {h=h-24;}
      hr            = String(h);if (hr.length()==1){hr="0"+hr;}
    }                    
    tijd[count]   = hr+pubdt.substring(p,p+3); //pubDate "13:23"
    pubdt         = tijd[count];    

    //<description> fill temp string
    desc[count]  = descr;
    message      = tijd[count]+" ";                                                       //"13:23 "

    // description starts with Ambulance use title instead
    // <title>a1 ambu 06127 - apeldoorn rit 113050 <description>Ambulance met spoed naar Apeldoorn type=2
   if (descr.indexOf("Ambulance")==0){  
     message     += "Ambulance ";     
     pos[8]       = title.indexOf("ambu");                 //in "a1 ambu 06126 - apeldoorn rit 105452 "
     pos[9]       = title.indexOf("rit");
     message     += title.substring(pos[8]+5,pos[8]+10);   //"06126"
     message     += " met ";
     if (urgent==false){message+="gepaste ";}
     message     += "spoed ";
     message     += title.substring(pos[9]  ,pos[9]+10);   //"rit 105452"
    } else {
      message     += descr;  
    } 

    // if <description> tells nothing use <item>
    if (descr=="Politie naar Apeldoorn") {                  //description is "Politie naar Apeldoorn"
      message =   tijd[count]+" ";  
      message     += title;}                                 //<title></title>

    // replace some texts 
    ReplaceThis(message," oc"," operationeel coordinator");
    message     = RepStr;
    ReplaceThis(message," od"," officier van dienst");
    message     = RepStr;
    ReplaceThis(message," mk"," meldkamer");
    message     = RepStr;            
    ReplaceThis(message,"naar Apeldoorn","");
    message     = RepStr;
    ReplaceThis(message,"in Apeldoorn","");
    message     = RepStr;    
    ReplaceThis  (message,"à","a");
    message     =   RepStr;
    ReplaceThis  (message,"é","e");
    message     =   RepStr; 
    ReplaceThis  (message,"ë","e");
    message     =   RepStr; 
    ReplaceThis  (message,"ï","i");
    message     =   RepStr;               

    // select led color & set type
    mtype[count]=green;
    if (descr.indexOf ("Ambulance")!=-1||title.indexOf("ambu")   !=-1){mtype[count]=yellow;}
    if (descr.indexOf ("Politie")  !=-1||descr.indexOf("politie")!=-1){mtype[count]=blue;}
    if (descr.indexOf ("Brandweer")!=-1||descr.indexOf("brand")  !=-1||title.indexOf("bon")    !=-1){mtype[count]=red;}
    if (descr.indexOf ("Trauma")   !=-1||title.indexOf("trauma") !=-1){mtype[count]=magenta;}   
    GetMsgType        (mtype[count]);// text to String MessageType
    if (prio!=0)      {message += "PRIO " + String (prio);}
    scrol[count] =    message; 
    
    // display final data
    Serial.println    ("Melding "+String(count)+":"+tijd[count]+" type="+MessageType);  //pubDate
    Serial.print      ("<title>"+titl[count]+" <description>");                         //title
    Serial.print      (desc[count]);                                                    //description
    Serial.println    (" type="+ String(mtype[count]));
    Serial.println    (scrol[count]);                                                   //short description
    //Serial.println    ();
    // save a copy for when all messages  are non urgent
    if (allcount==0){        
      tijd [11]=tijd [0];
      titl [11]=titl [0];
      desc [11]=desc [0];
      scrol[11]=scrol[0];
      mtype[11]=mtype[0];
    }    
    allcount++;
    if   (urgent==true) {Serial.println("found URGENT event!")                ;Serial.println();count++;}
    else                {Serial.println("Skipping this non urgent event ....");Serial.println();} //overwrite non urgent messages   
    if   (allcount==10) {
      Serial.println    ("urgent events          :"+String(count));
      Serial.println    ("new urgent events      :"+String(new_urgent));
      break;
    }
    pos[2]      =       payload.indexOf("<title>" ,pos[7]);
    if (debug)          {Serial.println("foud at:"+String(pos[2]));}

} while (pos[2]!=-1);

    if (count<1){
      mtype[0]  =   mtype[11];
      scrol[0]  =   scrol[11];
      tijd [0]  =   tijd [11];
      titl [0]  =   titl [11];
      desc [0]  =   desc [11]; 
      count=1;
    }
    if (use_led>1){SetMyLed(uit);}
}

void getWebData(bool scanonly){ 
  if (use_led>1)        {SetMyLed(cyan);}
  //if ((WiFi.status() != WL_CONNECTED)) { }    // Check the current connection status
  WiFiClientSecure httpsClient;                 // Declare object of class WiFiClient
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);                // 15 Seconds
  delay(1000);
  Serial.print              ("HTTPS Connecting ... ");
  int r=0;                  //retry counter
  while                     ((!httpsClient.connect(host,httpsPort)) && (r < 25)){delay(100);Serial.print(".");r++;}
  if(r==25) {Serial.println ("Connection failed");}
  else      {Serial.println ("Connected to web") ;}
  Serial.print              ("requesting URL: ");
  Serial.print              (host+Link);
  httpsClient.print         (String("GET ") + Link + " HTTP/1.1\r\n" +
                            "Host: " + host + "\r\n" +               
                            "Connection: close\r\n\r\n");
  Serial.print              (" request sent");                  
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r")       {
      Serial.print          (" headers received ");
      break;
    }
  }
  Serial.println            ("reply was:");
  Serial.println            ("==========");
  String        line;
  int           linenr=0;
  payload =     "";
  while(httpsClient.available()){ 
    if (!scanonly&&use_led>0){BlinkPixels("apple");}       
    line  =     httpsClient.readStringUntil('?');  //Read Line by Line
    if          (linenr<3) {
       if(debug){Serial.println(line);} //Print response
    }  else     {Serial.println(line);}
    linenr++;
    if (linenr>13){break;}
    if (linenr>2){
      payload += line;
    //payload += "?";
    }
    if (scanonly&&linenr==4){break;}//read 2nd line too to get 1st pubdate io creationdate
   }
  Serial.println          ("==========");
  if (debug)              {Serial.println("closing connection");}  
  Serial.println          ();
  if (debug){ 
  //Serial.println(payload); //Print page does not work when string 2 long
    Serial.println(payload.length()); //Print page
    Serial.println(payload.substring(   0,500)); 
    Serial.println(payload.substring( 500,1000)); 
    Serial.println(payload.substring(1000,1500)); 
    Serial.println(payload.substring(1500,2000));
    Serial.print  ("payload.length()=");Serial.println(payload.length());    
  } 
  pos[0]      =  payload.indexOf  ("<lastBuildDate>");
  pos[1]      =  payload.indexOf  ("</lastBuildDate>" ,pos[0]);
  creation    =  payload.substring(pos[0]+lastBuildDate.length(),pos[1]-skipplus);
  //use 2nd line too to get 1st pubdate io creationdate
  pos[6]      =  payload.indexOf  ("<pubDate>"     ,pos[1]);
  pos[7]      =  payload.indexOf  ("</pubDate>"    ,pos[6]);
  pubdate     =  payload.substring(pos[6]+HTML_pubdate.length(),pos[7]-skipplus);
  if (use_led>1)            {SetMyLed(uit);}    
  if (!scanonly&&use_led>0) {BlinkPixels("uit");}       
  if (debug){
    Serial.print            ("Read from web creation :");
    Serial.print            (creation);
    Serial.println          ("<("+String(creation.length())+")");
    Serial.print            ("Read from web pubdate  :");
    Serial.print            (pubdate);
    Serial.println          ("<("+String(pubdate.length())+")");
  }
}

#if dosleep 
 void readfromEeprom(){
   if (forget==false){
     EEPROM.begin         (eepsize); 
     if                   (use_led>1&&MyRGBled){SetMyLed(magenta);}
     address       =      0;
     readByte             (address);
     eepcount      =      readbyte;
     readByte             (address);
     sitcount      =      readbyte;
     readByte             (address);
     old_Thour     =      readbyte;
     if (old_Thour>23)    {old_Thour=23;}
     readWord             (address);
     last_creation =      readword;
     readWord             (address);
     last_pub      =      readword;
     readWord             (address);
     WTXT          =      readword;
     if (WTXT.indexOf     ("Het weer in Apeldoorn")!=0){WTXT="";}
     Serial.println       ("Getting eeprom eepcount:"+String(eepcount));
     Serial.println       ("Getting eeprom sitcount:"+String(sitcount));
     Serial.println       ("Getting old_Thour      :"+String(old_Thour));
     Serial.print         ("Getting last_creation  :");
     Serial.print         (last_creation+"<");
     Serial.println       ("("+String(last_creation.length())+")");
     Serial.print         ("Getting last_pubdate   :");
     Serial.print         (last_pub+"<");
     Serial.println       ("("+String(last_pub.length())+")");
     Serial.print         ("Getting WTXT_weather   :");
     Serial.print         (WTXT+"<");
     Serial.println       ("("+String(WTXT.length())+")");
     if                   (use_led>1&&MyRGBled){SetMyLed(uit);}
   } // else {eepcount=0;sitcount=0;}// eeprom counter alredy set
  }

  void Write2Eeprom(){
    int lcl = last_creation.length();
    if (lcl>20){      
      if                  (use_led>1&&MyRGBled){SetMyLed(magenta);}
      EEPROM.begin        (eepsize); 
      address      =      0;
      writeByte           (address,eepcount);
      writeByte           (address,sitcount);
      writeByte           (address,Thour);
      writeWord           (address,last_creation);  
      writeWord           (address,last_pub);
      if (WTXT=="")       {WTXT="NO DATA";}  
      writeWord           (address,WTXT);   
      Serial.println      ("Writing new eepcount   :"+String(eepcount));
      Serial.println      ("Writing new sitcount   :"+String(sitcount));
      Serial.println      ("Writing Thour          :"+String(Thour));
      Serial.print        ("Writing last_creation  :");
      Serial.print        (last_creation+"<");
      Serial.println      ("("+String(last_creation.length())+")");      
      Serial.print        ("Writing last_pub       :");
      Serial.print        (last_pub+"<");
      Serial.println      ("("+String(last_pub.length())+")");      
      if                  (use_led>1&&MyRGBled){SetMyLed(uit);}
     }
  }

  void ByeBye(){
    Serial.println        ("Going to sleep now      zzzzzzzz"); 
    Serial.println        ();
    if                    (use_led>1&&MyRGBled){SetMyLed(uit);}//might not work because some pin stays up during sleep 
    delay                 (500);
    ESP.deepSleep         (sleepingtime);
  }

  void writeByte          (int adr,byte by) {
    EEPROM.write          (adr,by);
    EEPROM.commit         ();
    address++; 
    if (debug) {
      Serial.print        ("writeByte ");
      Serial.print        (String  (adr)+" ");
      Serial.println      (by);
    }
  }

  void readByte           (int adr) {
    readbyte      =       (EEPROM.read(adr));
    address++; 
    if (debug)            {
      Serial.print        ("readByte ");
      Serial.print        (String (adr)+" ");
      Serial.println      (readbyte);}
  }
  
  void writeWord          (int adr,String word) {
    slen          =       word.length();
    delay                 (10);
    for                   (loc=0;loc<slen;++loc) {
      EEPROM.write        (adr,word[loc]);
      adr++;
    }
    EEPROM.write          (adr, '\0');
    EEPROM.commit         ();
    address       =       adr+1;
  }

  void readWord(int adr) {
    readword      =       "";
    char readChar =       'X';
    while                 (readChar != '\0') {
      readChar    =       char(EEPROM.read(adr));
      delay               (10);
      adr++;
      if                  (readChar!='\0'){readword += readChar;}
    }
    address       =       adr;
    if (debug){
      Serial.print        (readword);
      Serial.print        (" (");
      Serial.print        (readword.length());
      Serial.println      (")");
    }
  }
#endif

#if MyRGBled
  void SetMyLed(int color){
    if (color==red)    {analogWrite(red,1024);analogWrite(green,LOW );analogWrite(blue, LOW);}    
    if (color==green)  {analogWrite(red,LOW );analogWrite(green,1024);analogWrite(blue, LOW);}
    if (color==blue)   {analogWrite(red,LOW );analogWrite(green,LOW );analogWrite(blue,1024);}
    if (color==yellow) {analogWrite(red,1024);analogWrite(green,1024);analogWrite(blue, LOW);}
    if (color==magenta){analogWrite(red,LOW );analogWrite(green,1024);analogWrite(blue,1024);}
    if (color==cyan)   {analogWrite(red,1024);analogWrite(green,LOW );analogWrite(blue,1024);}
    if (color==white)  {analogWrite(red,1024);analogWrite(green,1024);analogWrite(blue,1024);}
    if (color==uit)    {analogWrite(red,LOW );analogWrite(green,LOW );analogWrite(blue,LOW );}  
  }
#else
  void SetMyLed(int color){
      int mypixel=0;
      pixels.setPixelColor  (mypixel, pixels.Color(colors[(color*3)],colors[(color*3)+1],colors[(color*3)+2]));
      pixels.show();
      if (debug){Serial.println  ("neopixel "+String(colors[(color*3)])+" "+String(colors[(color*3)+1])+ " "+String(colors[(color*3)+2]));}
  }
  
  void BlinkPixels(String color){
    flop=!flop;  
    if (flop) {SetPixel(0,color);SetPixel(1,"uit");}  
    else      {SetPixel(1,color);SetPixel(0,"uit");}  
  }
  
  void SetPixel(int p,String color){
    for (int i=0;i<14;i++){
      if    (color==names[i]){
        int r=colors[(i*3)];
        int g=colors[(i*3)+1];
        int b=colors[(i*3)+2];
        pixels.setPixelColor  (p, pixels.Color(r,g,b));
        pixels.show();
        if (debug){Serial.println ("RGB="+String(r)+","+String(g)+","+String(b));}
        break;
      }
    }
  }
#endif

void GetMsgType  (int typ){
  MessageType      =     "onbekende soort";
  if (typ==red    ){MessageType="Brandweer";}
  if (typ==blue   ){MessageType="Politie";  }
  if (typ==yellow ){MessageType="Ambulance";}
  if (typ==magenta){MessageType="Trauma";   }
}

void ReplaceThis(String S0,String S1,String S2){
  if (debug){Serial.println("message :" + S0);}        
  int sta1 =  S0.indexOf(S1);
  if (sta1==-1){RepStr=S0;return;}
  pos[8]  = S0.length();  
  pos[9]  = S1.length();  
  RepStr  = S0.substring(0,sta1);
  RepStr += S2;
  RepStr += S0.substring(sta1+pos[9],pos[8]+1);
  if (debug){
    Serial.println("sta1  :"+String(sta1));
    Serial.println("pos[8]:"+String(pos[8]));
    Serial.println("pos[9]:"+String(pos[9]));
    Serial.println("replaced " + S1 + " by " + S2);        
    Serial.println("RepStr  :" + RepStr);        
  }    
}

void getDST(int t){ //needs Tyear,Tday,Tmont,Tday 
  Tyear=t;
  if (debug){
    Serial.print      ("Finding  summertime period for ");
    Serial.println    (Tyear);
  }
  getDOWyear        (Tyear);    // sets value of "offset" = day of week of 1/1/Tyear
  getLEAP           (Tyear);    // check if this year is a leap year sets bool LEAP
  StartDST=90+LEAP; // if bool LEAP == true then StartDST=91
  for (int test=90+LEAP; test>83+LEAP; test--) { // find last sunday in march
    getDOW(test);
    if (DOW==0){break;}
    StartDST--;
  }
  EndDST=304+LEAP;
  for       (int test=304+LEAP; test>287+LEAP; test--) { // find last sunday in october
    getDOW  (test);
    if      (DOW==0){break;}
    EndDST--;
  }
  today = 0;
  for       (mon=1; mon<Tmonth; mon++) { // get current day of year
    today+=monthdays[mon-1];
  }
  today=today+Tday;
  if (today>=StartDST&&today<EndDST){DST=true;} else {DST=false;} 
  if (debug){
    Serial.print      ("Today is day ");
    Serial.print      (today);
    Serial.print      (" of ");
    Serial.print      (Tyear);
    Serial.print      (" so it's ");
    if (DST)          {Serial.println(" summertime!");} 
    else              {Serial.println("wintertime...");} 
  }
}

// check if year is a leap and set bool LEAP
void getLEAP(int y){ // leapyear div 4 or 400 but not 100 has 366 days io 365
  bool test1 = ((y-((y/400)*400))==0);  
  bool test2 = ((y-((y/100)*100))==0);  
  bool test3 = ((y-((y/4)*4))==0);  
  if (test1)    {LEAP=true;} 
  else 
    {if (test2) {LEAP=false;}
    else 
      {if (test3){LEAP=true;} 
      else {LEAP=false;} 
      }
    }
  if (LEAP){
    if (debug){
      Serial.print      (y);
      Serial.print      (":");
      Serial.println    ("leapyear!");      
    }
    monthdays[1]=29;
  } else {
    monthdays[1]=28;}
}

// get the day of the week for 1st day of year
void getDOW(int d){
  int h = (d-1+offset)/7;
  DOW   =  d-1+offset - h*7;
  mon = 0;
  while (d>monthdays[mon]){
    d=d-monthdays[mon];
    mon++;
  }
  daynr=d;
}

void getDOWyear(int y){ // changes LEAP
  offset=6; // 202200101 = saturday = 6
  if (y==2022){DOW=offset;}
  else {
    for (checkyear = 2023; checkyear<=y;checkyear++){
      getLEAP(checkyear-1);
      if (LEAP){offset=offset+366;} else {offset=offset+365;}
      DOW   =  offset - 7*(offset/7);
    }  
  }  
  offset=DOW;// set first day of week for this year
}

// { "liveweer": [{"plaats": "Apeldoorn", "temp": "13.8", "gtemp": "11.2", "samenv": "Licht bewolkt", "lv": "75", "windr": "ZZW", "windrgr": "202", "windms": "7", "winds": "4", "windk": "13.6", "windkmh": "25.2", "luchtd": "1023.6", "ldmmhg": "768", "dauwp": "9", "zicht": "33", "verw": "Af en toe zon en droog", "sup": "07:45", "sunder": "16:54", "image": "lichtbewolkt", "d0weer": "halfbewolkt", "d0tmax": "14", "d0tmin": "9", "d0windk": "3", "d0windknp": "10", "d0windms": "5", "d0windkmh": "19", "d0windr": "ZW", "d0windrgr": "225", "d0neerslag": "0", "d0zon": "33", "d1weer": "halfbewolkt", "d1tmax": "15", "d1tmin": "8", "d1windk": "2", "d1windknp": "6", "d1windms": "3", "d1windkmh": "11", "d1windr": "ZW", "d1windrgr": "225", "d1neerslag": "10", "d1zon": "50", "d2weer": "halfbewolkt", "d2tmax": "14", "d2tmin": "5", "d2windk": "2", "d2windknp": "4", "d2windms": "2", "d2windkmh": "7", "d2windr": "ZO", "d2windrgr": "135", "d2neerslag": "10", "d2zon": "60", "alarm": "0", "alarmtxt": ""}]} 

void getWeatherData(){  
if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    WiFiClient client;
    HTTPClient http;
    http.begin(client,KNMI);           //Specify the URL for KNMI
    int httpCode = http.GET();  //Make the request 
    if (httpCode > 0) { //Check for the returning code 
         payload = http.getString();
         if (debug){
         Serial.println(httpCode);
         Serial.println();
         Serial.println(payload);}        
     } else { Serial.println("Error on HTTP request");WTXT="";return;}
    http.end(); //Free the resources  

    Serial.println(payload.substring(   0,500)); 
    Serial.println(payload.substring( 500,1000)); 
//  Serial.println(payload.substring(1000,1500)); 
    Serial.print  ("payload.length()=");Serial.println(payload.length());    
    Serial.println();

//  Het weer in Apeldoorn 17.3 graden max 15 licht bewolkt wind Oost1 verwachting zonnig. vannacht vrij koud en plaatselijk mist morgen halfbewolkt 2 tot 11 graden. (160)
    WTXT     =  "Het weer in Apeldoorn om ";
    WTXT    += TimeStamp.substring(0,5);        //16:56 io 16:56:20
    WTXT    +=  " ";
    FindThis   ("temp");
    WTXT    +=  found;
    WTXT    +=  " graden tot ";
    FindThis   ("d0tmax");
    WTXT    +=  found;
    WTXT    +=  " ";
    FindThis   ("samenv");
    found.toLowerCase();
    WTXT    +=  found;
    WTXT    +=  " wind ";
    FindThis   ("windr");
    WTXT    +=  found;
    WTXT    +=  " ";
    FindThis   ("winds");
    WTXT    +=  found;
    WTXT    +=  " morgen ";
    FindThis   ("d1weer");
    WTXT    +=  found;
    WTXT    +=  " ";    
    FindThis   ("d1tmin");
    WTXT    +=  found;
    WTXT    +=  " tot ";
    FindThis (  "d1tmax");
    WTXT    +=  found;
    WTXT    +=  " graden.";
    FindThis   ("alarm");
    if (found=="1"){FindThis ("alarmtxt");WTXT+=" ALARM:";WTXT+=found;}

    /*{ "liveweer": [{"plaats": "Apeldoorn", "temp": "14.2", "gtemp": "11.6", "samenv": "Licht bewolkt", 
     "lv": "73", "windr": "ZW", "windrgr": "225", "windms": "7", "winds": "4", "windk": "13.6", "windkmh": "25.2", 
     "luchtd": "1023.8", "ldmmhg": "768", "dauwp": "9", "zicht": "35", "verw": "Af en toe zon en droog", "sup": "07:45", 
     "sunder": "16:54", "image": "lichtbewolkt", "d0weer": "halfbewolkt", "d0tmax": "14", "d0tmin": "9", "d0windk": "3", 
     "d0windknp": "10", "d0windms": "5", "d0windkmh": "19", "d0windr": "ZW", "d0windrgr": "225", "d0neerslag": "0", 
     "d0zon": "33", "d1weer": "halfbewolkt", "d1tmax": "15", "d1tmin": "8", "d1windk": "2", "d1windknp": "6", 
     "d1windms": "3", "d1windkmh": "11", "d1windr": "ZW", "d1windrgr": "225", "d1neerslag": "10", "d1zon": "50", 
     "d2weer": "halfbewolkt", "d2tmax": "14", "d2tmin": "5", "d2windk": "2", "d2windknp": "4", "d2windms": "2", 
     "d2windkmh": "7", "d2windr": "ZO", "d2windrgr": "135", "d2neerslag": "10", "d2zon": "60", "alarm": "0", 
     "alarmtxt": ""}]}  
     */

    if (RThour<12){morning=true;}else{morning=false;}
    //ochtend   Vandaag zon op 07:43 uur min 2 max 15 zon 65% droog wind zo4 ..... zon onder->middag
    if (morning){
      if (RThour<10){
        WTXT2    =  TimeStamp.substring(0,5);
        WTXT2   +=  " Vandaag zon op ";
        FindThis   ("sup");
        WTXT2   +=  found;    
        WTXT2   +=  " uur "; 
      } else {
        WTXT2    =  "Vandaag ";
      }
      FindThis   ("d0tmin");
      WTXT2   +=  found;
      WTXT2   +=  " tot "; 
      FindThis   ("d0tmax");
      WTXT2   +=  found;
      WTXT2   +=  " graden "; 
      FindThis   ("d0zon");
      if (found=="0"){
        WTXT2   +=  "bewolkt ";
      }else{
        WTXT2   +=  "zon "; 
        WTXT2   +=  found;
        WTXT2   +=  "% "; 
      }
      FindThis   ("d0neerslag");
      if (found=="0"){
        WTXT2   +=  "droog ";
      }else{
        WTXT2   +=  "neerslag "; 
        WTXT2   +=  found;
        WTXT2   +=  "% "; 
      }
      WTXT2   +=  "wind "; 
      FindThis   ("d0windr");
      WTXT2   +=  found;
      WTXT2   +=  " "; 
      FindThis   ("d0windk");
      WTXT2   +=  found;
  
    } else {
      //middag   17:09 Nu 25 graden ZW3 zon onder 07:43 morgen min 2 max 15 graden zon 65% droog wind zo4 
      WTXT2    =  TimeStamp.substring(0,5);
      WTXT2   +=  " nu ";
      FindThis   ("temp");
      WTXT2   +=  found;
      WTXT2   +=  " graden wind "; 
      FindThis   ("windr");
      WTXT2   +=  found;
      FindThis   ("winds");
      WTXT2   +=  found;
      if (RThour<22){
        WTXT2   +=  " zon onder "; 
        FindThis   ("sunder");
        WTXT2   +=  found;
        WTXT2   +=  " uur. ";     
      }
      WTXT2   +=  "Morgen "; 
      FindThis   ("d1tmin");
      WTXT2   +=  found;
      WTXT2   +=  " tot "; 
      FindThis   ("d1tmax");
      WTXT2   +=  found;
      WTXT2   +=  " graden "; 
      FindThis   ("d1zon");
      if (found=="0"){
        WTXT2   +=  "bewolkt ";
      }else{
        WTXT2   +=  "zon "; 
        WTXT2   +=  found;
        WTXT2   +=  "% "; 
      }
      FindThis   ("d1neerslag");
      if (found=="0"){
        WTXT2   +=  "droog ";
      }else{
        WTXT2   +=  "neerslag "; 
        WTXT2   +=  found;
        WTXT2   +=  "% "; 
      }
      WTXT2   +=  "wind "; 
      FindThis   ("d1windr");
      WTXT2   +=  found;
      WTXT2   +=  " kracht "; 
      FindThis   ("d1windk");
      WTXT2   +=  found;
  }//middag
      
  } else {Serial.println("WiFi Disconnected");}  
}

void FindThis(String dit){
    dit          = "\"" + dit + "\"";
    int     len0  = dit.length()+1;
    int     pos0  = payload.indexOf   (dit)+len0;
    String  dat   = payload.substring (pos0,pos0+200);
    int     pos1  = dat.indexOf       ("\"");
    int     len2  = dat.length        ()-pos1;
            dit   = dat.substring     (pos1+1,len2);
    int     pos2  = dit.indexOf       ("\"");
    found         = dit.substring     (0,pos2);
    if (debug){
      Serial.println  ("zoek   :"+dit);
      Serial.println  ("pos0:"+String(pos0));
      Serial.println  ("dat :"+dat);
      Serial.println  ("pos0:"+String(pos0));
      Serial.println  ("pos1:"+String(pos1));
      Serial.println  ("len2:"+String(len2));
      Serial.println  ("dit :"+dit);  
      Serial.println  (found);
    }
}

void SkipCodes(String S3){      //&#8216;Lege&#8217; Sebastian Vettel zegt F1 gedag: &#8216;Weet waarschijnlijk nog niet hoe hard ik dit ga missen&#8217;
   pos[0]    = S3.indexOf("&#");
   if(pos[0]!=-1){
     do {
       if (debug){Serial.println("fnd:" + String(pos[0])+":"+S3.substring(pos[0],pos[0]+7)); }       
       String S1 = S3.substring(0,pos[0]);
       String S2 = S3.substring(pos[0]+7,S3.length()+1);
       S3        = S1+S2;
       pos[0]    = S3.indexOf("&#");
       if (debug){Serial.println("S3:" + S3);}        
    } while (pos[0]!=-1);
  } 
  RepStr = S3;
}

void GetSomeNews(int site){
  String  ADCData, getData, Link;
  int     adcvalue  = analogRead(A0);   //Read Analog value of LDR
  ADCData = String(adcvalue);           //String to integer conversion
  Link    = Links[site];   
  WiFiClientSecure httpsClient;         //Declare object of class WiFiClient
  if (site==0){httpsClient.setFingerprint  (fingerprint0);}
  if (site==1){httpsClient.setFingerprint  (fingerprint1);}
  if (site==2){httpsClient.setFingerprint  (fingerprint2);}
  if (site==3){httpsClient.setFingerprint  (fingerprint3);}
  if (site==4){httpsClient.setFingerprint  (fingerprint4);}
  if (site==5){httpsClient.setFingerprint  (fingerprint5);}  
  if (site==6){httpsClient.setFingerprint  (fingerprint6);}   
  if (site==7){httpsClient.setFingerprint  (fingerprint7);}      
  if (site==8){httpsClient.setFingerprint  (fingerprint8);}      
  httpsClient.setTimeout      (15000);   // 15 Seconds
  delay(1000);
  //connect
  if (debug){Serial.println();Serial.print("HTTPS Connecting to ");
  Serial.print(hosts[site]);}
  int r=0;                               //retry counter
  while ((!httpsClient.connect(hosts[site], httpsPort)) && (r < 25))
  {delay(100);Serial.print(".");r++;}     
  if(r==25) {Serial.println(" Connection failed");failed=true;return;}
  else      {  if (debug){Serial.println(" Connected to web ");}}
  //GET Data
  if (debug){
    Serial.print      ("requesting URL         : ");
    Serial.println    (hosts[site]+Link);  
  }
  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + hosts[site] + "\r\n" +               
               "Connection: close\r\n\r\n");
  if    (debug){Serial.print  ("request sent ");}                  
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
        if (debug){Serial.print("headers received ");}
      break;
    }
  }
  if (debug){
    Serial.println("reply was:");
    Serial.println("==========");
    Serial.print  ("divider=");
    Serial.println(divider[site]);
  }
  String line;
  int    linenr=0;
  payload="";  
  //read lines
  while(httpsClient.available()){ 
    line = httpsClient.readStringUntil(divider[site]);  
    if (debug){Serial.println(line);}
//    payload += "+";
    payload += line;
    payload += divider[site];
    linenr++;
    if (linenr>lines[site]){break;}    
  }
  if (debug){
    Serial.print  ("closing connection. Received ");  
    Serial.print  (payload.length());       //Print page 
    Serial.println(" bytes.");  
    Serial.println(payload);                //Print page
  }
  pos[7] = payload.indexOf(StartWith[site]);
  pos[2] = payload.indexOf("<title>"       ,pos[7]);
  pos[3] = payload.indexOf("</title>"      ,pos[2]);
  pos[4] = payload.indexOf("<description>" ,pos[3]);
  pos[5] = payload.indexOf("</description>",pos[4]);
  pos[6] = payload.indexOf("<pubDate>"     ,pos[5]);
  pos[7] = payload.indexOf("</pubDate>"    ,pos[6]);
  descr  = payload.substring(pos[4]+HTML_description.length(),pos[5]);//<description>descr</description>
  title  = payload.substring(pos[2]+HTML_title.length()      ,pos[3]);//<title>title</title>
  pubdt  = payload.substring(pos[6]+HTML_pubdate.length()    ,pos[7]);//pubDate "Thu, 06 Oct 2022 11:23:20 +0000"

  if (pos[2]==-1||pos[3]==-1) {title="Not found!";}
  if (pos[4]==-1||pos[5]==-1) {descr="Not found!";}
  if (pos[6]==-1||pos[7]==-1) {pubdt="Not found!";}
  
  if (debug){
    Serial.println  ("<title>"+title);
    Serial.println  ("<descr>"+descr);
    Serial.println  ("<pubdt>"+pubdt);
    Serial.println  ();
    
  }
    ReplaceThis  (title,"<![CDATA[","");
    title     =   RepStr;
    ReplaceThis  (title,"]]","");
    title     =   RepStr;
    ReplaceThis  (title,">","");
    title     =   RepStr;
    ReplaceThis  (title,"‘","");
    title     =   RepStr;
    ReplaceThis  (title,"’","");
    title     =   RepStr; 
    ReplaceThis  (title,"'","");
    title     =   RepStr; 
    ReplaceThis  (title,"à","a");
    title     =   RepStr;
    ReplaceThis  (title,"é","e");
    title     =   RepStr; 
    ReplaceThis  (title,"ë","e");
    title     =   RepStr; 
    ReplaceThis  (title,"ï","i");
    title     =   RepStr; 
    ReplaceThis  (title,"ü","u");
    title     =   RepStr; 
    ReplaceThis  (title,"ö","o");
    title     =   RepStr; 
    ReplaceThis  (title,"•","*");
    title     =   RepStr; 
    ReplaceThis  (title,"‘","");
    title     =   RepStr; 
    ReplaceThis  (title,"­","");
    title     =   RepStr; 
      
    ShortHost=hosts[site];
    if (ShortHost.indexOf("www."  )==0){ShortHost=ShortHost.substring(4,20);}    
    if (ShortHost.indexOf("feeds.")==0){ShortHost=ShortHost.substring(6,20);}    
    if (ShortHost=="sportnieuws.nl"){//remove some junk
      SkipCodes     (title);
      title     =    RepStr;
      ReplaceThis   (title,"🎥​ ","");
      title     =   RepStr;
      ReplaceThis   (title,"🎥​​","");
      title     =   RepStr;
      if (debug)    {Serial.println(title);}  
    }

     message   =     ShortHost;

     if       (ShortHost==  "verkeerplaza.nl"){ 
       GetTraffic           ();
       message         +=   APELDOORN;
       if        (count==0) {message += "Geen files rond Apeldoorn ###";}
     } else   {
       message   +=         " *** ";
       message   +=         title;
       message   +=         " *** ";
       message   +=         ShortHost;          
     }

    Serial.println          ("Now displaying:"+message);  
    char  W0 [message.length()+1];
    message.toCharArray     (W0, message.length()+1);
    myDisplay.begin         ();
    myDisplay.setIntensity  (0);// Set the intensity (brightness) of the display (0-15):
    myDisplay.displayClear  ();
    myDisplay.displayText   (W0, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    while (!myDisplay.displayAnimate()){delay(5);}
    myDisplay.displayReset();  
} 

void GetTraffic(){
    APELDOORN   =   " ### ";
    pos[1]      =   0;
    count       =   0;
    allcount    =   0;
    pos[2]      =   payload.indexOf("<title>" ,pos[1]);
    do {
      pos[3] = payload.indexOf  ("</title>"      ,pos[2]);
      pos[4] = payload.indexOf  ("<description>" ,pos[3]);
      pos[5] = payload.indexOf  ("</description>",pos[4]);    
      descr  = payload.substring(pos[4]+HTML_description.length(),pos[5]);
      title  = payload.substring(pos[2]+HTML_title.length()      ,pos[3]);
      pos[2] = payload.indexOf  ("<title>" ,pos[5]);
      if (title.indexOf("Apeldoorn")!=-1||title.indexOf("Arnhem")!=-1){
          count++;
          ReplaceThis  (title,"&gt; ","- ");
          title     =   RepStr;
          if (title.length()<=50){
              titl[count]=title+": "+descr;              
          } else {
              titl[count]=title;
          }
          APELDOORN    +=   titl[count];
          APELDOORN    +=   " ### ";
          if (debug){
            Serial.println    ("<item>"+String(count));
            Serial.println    (titl[count]);
            Serial.println    ();            
          }
      }    
      allcount++;
    } while (pos[2]!=-1);  
}
