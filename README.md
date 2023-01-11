# ticker
emergency wheather and news ticker
This is an D1-mini based ticker that shows (dutch) emergency services messages and local traffic, weather and (rss) news services on a scrolling MAX72XX display.

Click picture for 8 seconds Youtube video<BR> 
[![video](https://github.com/gtmans/ticker/blob/main/tickerYT.jpg)](https://youtu.be/rS8Z2R228mY)

What it does
The D1-mini wakes up once every minute and checks an .rss feed of local emergency services (in my case https://alarmeringen.nl//feeds/city/apeldoorn.rss) and whenever there is an emergency message of
Police, Firebrigate or Ambulance it creates a siren sound on the buzzer (when switched on), sends a (prowl) message to my phone, flashes the leds in (blue/red/yellow or purple color) and displays the message on the ticker display.
When there are no emergency messages for 2 minutes it displays some other messages like local weather info (http://weerlive.nl/api/json-data-10min.php - you need a free api key), local traffic info (rss free verkeersplaza), or news from 1 of 5 other .rss feeds.
It is based on Dutch (P2000) emergency message system and weather services. You have to customise this depending on your own wishes and location but you can use it to read any https:// .rss feed.   

Parts
- Wemos D1 Mini V4 - ESP8266 - CH340
- SPI-I2C-UART Bi-Directional Logic Level Converter 4 channel
- active buzzer (optional)
- WS2812B Digital 5050 RGB LED Strip or RGB LED - 5mm (optional)
- 100kΩ Potmeter (Type RV09 or other)
- Breadboard Tactile Pushbutton Switch Momentary 2pin 6*6*5mm
- 3x Printboard Switch 8x8mm
- experimental printboard 4x6 cm
- experimental printboard 2x8 cm
- 30 cm flatcable
- 2x male + female Header Flatcable Connector 8p - 2x4p
- 2 x LED Matrix 32x8 met MAX7219 Module (or 1 or more)
- some 3D printed parts (optional)

Program

![layout](https://github.com/gtmans/ticker/blob/main/ticker-layout.png)

Change this settings according to your situation:
- #define prowl to 0 if you do not want to send message to prowl
- #define use_ticker to 0 if you do not want to use MAX72XX scrolling display
- #define myversion if you want to use 3 color RGB-led instead of Neopixel strip. The problem with the 3 color RGB-led is that the D1-mini does not have enough ports to use it together with buzzer so you have to choose one of the other versions. 
  If you do, look for "#if myversion==" to see which ports you have to connect to what.
- ssid and password of your local wifi
- your free api-key obtained at http://weerlive.nl/api/
- your prowl api key if you want to use prowl for messaging to your phone (https://www.prowlapp.com/ costs a litte once) 
- #define nrsites 9 to the number of .rss sites you want to use
- hosts[] (URL of HTTPS-site),links[] (rest of the URL),StartWith[](the place in the .rss file where you want to start searching),lines[] (#of lines you have to read in the .rss to get the first message) ,divider[] (deviding character which determines what "a line" is in the rss)
  Open the .rss or xml and examine how it is built up before you change this parameters. There is a test program you can run.
- fingerprint0-9[]. You can find the fingerprint when you click on the lock / certificate / SHA-1-vingerafdruk

Switches
- You can change the potmeter from 0 to half or full to change the number of emergency messages and more or less led debug info
- Switch for buzzer on/of
- Switch for led on/of
- Switch for ledmatrix on/of (does noet work 100%)



