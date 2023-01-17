 /* D1 mini pinout https://escapequotes.net/esp8266-wemos-d1-mini-pins-and-diagram/
  * S2 mini pinout https://www.espressif.com/sites/default/files/documentation/esp32-s2_datasheet_en.pdf
  * D1 mini           S2 mini 
  *
  * RST  -   TX     EN  1   -  40  39
  * A0   -   RX     3   2   -  38  37
  * D0   -   D1     5   4   -  36  25
  * D5   -   D2     7   6   -  34  33
  * D6   -   D3     9   8   -  21  18
  * D7   -   D4     11  10  -  17  16
  * D8   -   GND    12  13  -  GND GND
  * 3V3  -   5V     3V3 14  -  15  VBUS button=0 
  * 
  * https://alarmeringen.nl/feeds/province/gelderland.rss
  * https://alarmeringen.nl/feeds/city/apeldoorn.rss
  * https://alarmeringen.nl/gelderland/noord-en-oost-gelderland/apeldoorn/
  * https://112alarm.net/index.php
  * https://www.112-zuidland.nl/p1-en-a1-wat-betekend-dat-eigenlijk/
  * https://www.p2000alarm.nl/index.php?m=afkortingen
  * https://www.p2000monitor.eu/codes.html
  * https://www.firecom.nl/en/kennisbank/p2000-capcodes-vinden/
  * https://www.ad.nl/apeldoorn/rss.xml
  * https://www.destentor.nl/apeldoorn/rss.xml
  * 
  * https://roboticsbackend.com/arduino-push-button-tutorial/
  * buzzer https://www.circuitbasics.com/how-to-use-active-and-passive-buzzers-on-the-arduino/
  * eeprom 8266     https://hutscape.com/tutorials/eeprom-esp8266
  * level converter https://learn.sparkfun.com/tutorials/bi-directional-logic-level-converter-hookup-guide
  * https://circuits4you.com/2018/03/08/arduino-convert-string-to-character-array/
  * https://maakbaas.com/esp8266-iot-framework/logs/https-requests/* 
  * https://circuits4you.com/2019/01/10/esp8266-nodemcu-https-secured-get-request/
    //SHA-1=2F A5 F5 85 94 66 BC 89 25 E2 FA 21 D1 6F 5B B5 DB E4 55 F0
  * https://www.researchgate.net/post/How-to-reboot-reset-ESP32-without-pressing-the-reboot-button-on-the-board Connect any GPIO port to the RST pin.
  * https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
  * http://www.esp8266learning.com/wemos-mini-ws2812b-example.php
   
   08:54:49.191 -> xml version="1.0" encoding="utf-8"
   08:54:49.237 -> >
   08:54:49.237 -> <rss version="2.0" xmlns:atom="http://www.w3.org/2005/Atom"><channel><title>Alarmeringen.nl feed</title><link>http://alarmeringen.nl/gelderland/noord-en-oost-gelderland/apeldoorn/</link><description>Alarmeringen.nl: Alle alarmeringen voor: Apeldoorn</description><atom:link href="http://alarmeringen.nl/feeds/city/apeldoorn.rss" rel="self"></atom:link><language>nl-NL</language><lastBuildDate>Wed, 26 Oct 2022 06:40:29 +0000</lastBuildDate><item><title>a1 ambu 06126 - apeldoorn rit 107790</title><link>http://alarmeringen.nl/gelderland/noord-en-oost-gelderland/apeldoorn/42883288/p2000-ambulance-met-spoed-naar-apeldoorn.html
   08:54:49.284 -> ==========

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             <pubDate>Wed, 26 Oct 2022 06:40:29 +0000</pubDate>
 */
