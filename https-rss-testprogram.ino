/* https://circuits4you.com/2019/01/10/esp8266-nodemcu-https-secured-get-request/
 * HTTPS Secured Client GET Request
 * Copyright (c) 2019, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */

String TST="<title>A1 Amsterdam &gt; Amersfoort (5 km) Tussen Afrit Bunschoten en Afrit Barneveld</title><pubDate>Tue, 29 Nov 2022 06:32:27 GMT</pubDate><description>Stilstaand verkeer tussen Afrit Bunschoten en Afrit Barneveld</description></item><item><title>A1 Apeldoorn &gt; Amersfoort (3 km)Tussen Afrit Stroe en Afrit Hoevelaken</title><pubDate>Tue, 29 Nov 2022 07:03:29 GMT</pubDate><description>Langzaam verkeer tussen Afrit Stroe en Afrit Hoevelaken</description></item><item><title>A1 Apeldoorn &gt; Amersfoort (2 km)  Tussen Afrit Voorthuizen en Afrit Hoevelaken</title><description>Langzaam verkeer tussen Afrit Voorthuizen en Afrit Hoevelaken</description></item><item><title>A1 Apeldoorn &gt; Amersfoort (3 km)Tussen Afrit Stroe en Afrit Hoevelaken</title><description>Langzaam verkeer tussen Afrit Stroe en Afrit Hoevelaken</description></item><item><title>A50 Apeldoorn &gt; Arnhem</title><description>Noodsituatievoertuig. RWS Weginspecteur</description></item>";
String APELDOORN;

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#define ver 6

int         pos[12];
int         count;
int         allcount;
int         site;
int         sitenr;
String      HTML_description   = "<description>";
String      HTML_pubdate       = "<pubDate>";
String      HTML_title         = "<title>";
String      HTML_item          = "<item>";
String      descr;
String      title;
String      pubdt;
String      RepStr;
bool        debug=false;
bool        fail;
String      payload="";
String       titl [12]; 
String       desc [12]; 

/* Set these to your desired credentials. */
const char *ssid = "myWIFI";  //ENTER YOUR WIFI SETTINGS
const char *password = "secret123";

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

void setup() {  
  Serial.begin      (115200);
  while(!Serial);   // time to get serial running
  delay             (1000);

  WiFi.mode         (WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay             (1000);
  WiFi.mode         (WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("");

  for (sitenr=0;sitenr<nrsites;sitenr++){
    ShortHost=hosts[sitenr];
    if (ShortHost.indexOf("www.")==0){ShortHost=ShortHost.substring(4,20);}
    GetSomeNews(sitenr);
    if (!fail){
      ReplaceThis  (title,"<![CDATA[","");
      title     =   RepStr;
      ReplaceThis  (title,"]]","");
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
        if (ShortHost=="sportnieuws.nl"){
          SkipCodes     (title);
          title     =    RepStr;
          ReplaceThis  (title,"🎥​ ","");
          title     =   RepStr;      
          if (debug){Serial.println(title);}  
          pos[0]    =   descr.indexOf("</p>");
          RepStr    =   descr.substring(0,pos[0]);
          descr     =   RepStr;
          if (debug){Serial.println(descr);}

          ReplaceThis  (descr,"<![CDATA[<p>","");
          descr     =   RepStr;
          if (debug){Serial.println(descr);}
          ReplaceThis  (descr,"🎥​ ","");
          descr     =   RepStr;      

          SkipCodes     (descr);
          descr     =    RepStr;
          if (debug)    {Serial.println(descr);}      
      }
    Serial.println  (ShortHost);
    Serial.println  ("<title>"+title);
    Serial.println  ("<descr>"+descr);
    Serial.println  ("<pubdt>"+pubdt);
    Serial.println  ();
   } else {Serial.println  (ShortHost+" failed connection.");}//fail
   
     if       (ShortHost=="verkeerplaza.nl"){ 
       GetTraffic  ();
       if     (count>0) {Serial.println(APELDOORN);}
     else               {Serial.println("Geen files rond Apeldoorn ...");}
     }
  }
}
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
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
  if (debug){Serial.print("HTTPS Connecting to ");
  Serial.print(hosts[site]);}
  int r=0;                               //retry counter
  while ((!httpsClient.connect(hosts[site], httpsPort)) && (r < 25))
  {delay(100);Serial.print(".");r++;}     
  if(r==25) {Serial.println(" Connection failed");fail=true;return;}
  else      {  if (debug){Serial.println(" Connected to web ");fail=false;}}
  //GET Data
  if (debug){
    Serial.print("requesting URL: ");
    Serial.println(hosts[site]+Link);  
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

void SkipCodes(String S3){
     //&#8216;Lege&#8217; Sebastian Vettel zegt F1 gedag: &#8216;Weet waarschijnlijk nog niet hoe hard ik dit ga missen&#8217;
//    Serial.println(title);  
//<title>Eindelijk weer naar het WK darts: zo boek je betrouwbare tickets én de reis naar Ally Pally
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

/* sample data
 * <title>A1 Apeldoorn &gt; Amersfoort (2 km)  Tussen Afrit Voorthuizen en Afrit Hoevelaken</title>
 * <description>Langzaam verkeer tussen Afrit Voorthuizen en Afrit Hoevelaken</description> 
 * <title>A1 Apeldoorn &gt; Amersfoort (3 km)Tussen Afrit Stroe en Afrit Hoevelaken</title>
 * <description>Langzaam verkeer tussen Afrit Stroe en Afrit Hoevelaken</description>
 * <title>A50 Apeldoorn &gt; Arnhem</title>
 * <description>Noodsituatievoertuig. RWS Weginspecteur</description>
 *<title>A1 Amsterdam &gt; Amersfoort (5 km) Tussen Afrit Bunschoten en Afrit Barneveld</title><pu
 */
void GetTraffic(){
    APELDOORN   =   "### ";
//  payload     =   TST;
//  Serial.println    (payload);
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
