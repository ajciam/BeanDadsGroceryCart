# BeanDadsGroceryCart
Software files are Ardunio Sketches and Examples used.

Main files are in Bean Dads Grocery Cart.

Was setup in VS Code with the Platform IO extension. 

Boards used: ESP32 Cam, Arudnio MEGA

For platform IO Setup see: https://www.youtube.com/watch?v=JmvMvIphMnY&t=2703s and https://dronebotworkshop.com/platformio/

Platform IO works similary to the Ardunio IDE, but as an IDE for many microcontorllers with added features from the VS Code platform (libbary mangment, auto complete,
hover info etc.)

It has some differneces versus the Ardunio IDE mainly

1) you must include 

  #include <Arduino.h>

  at the top of your sketches. Sketches are called projects in Platform IO 
  
2) Functions must be defined before they are called (like most progrmaing lagnugaes, the arudnio IDE is the exception)
3) No serial plotter, if desired you can launch and use the serial plotter from the Ardunio IDE
4) platformio.ini acts as the config file. You can change Baudrates and other things there like which board is running
5) Libaries must be called in each proejct file. In the Arudnio IDE calling a libaray once makes it avaliable for all skecthes
6) Files are saved so .cpp files versus Ardunio IDE .ino. They work the same.


For example Structure go to https://github.com/platformio/platform-atmelavr/tree/develop/examples


Madeleine's Files:
esp32cam-jpegpush pushes a jpeg to the internet which then accessible via an image URL that Arduino gives you. 
The image is available at the url "http://{Local IP}/cam-lo.jpg".
cam-mid.jpg and cam-hi.jpg also work but in my experience the low resolution image updates faster.
We're going to be using the cart over the utexas-iot wifi because utexas isn't accessible by esp32 without some way to input a UT EID. If you want to use this wifi you need to go to network.utexas.edu
and register all devices, including the esp32 and any devices that want to access the online image.
Madeleine's esp32 and thinkpad are currently registered with utexas-iot, and the wifi credentials are below:
const char* WIFI_SSID = "utexas-iot";
const char* WIFI_PASS = "59551002838320796686";
(these correspond to lines 5-6, which can be adjusted if you want to connect to a different wi-fi network)

pythonfiles contains a few avenues for pulling the image from the internet and doing some sort of processing with it
current avenues for exploration:
Barcode Reader
Color Detection
Google's Vision API (very promising)
Haar Cascade (out of scope, but trained models are available online for common things such as cars and faces)

The plan is to develop a python application and launch it to mobile using Beeware.