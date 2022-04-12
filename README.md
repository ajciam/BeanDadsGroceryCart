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
