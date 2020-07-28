**NodeMCU openweathermap** 

![live picture](../lib/IMG_20200728_211318780.jpg)

This project read the weather information from <a href="https://openweathermap.org/" target="_blank">openweathermap</a> and disply on 2x16 LCD.
Please remember to modify the project you need to edit the code inside the platformio and compile it.

**Getting Start**

Install [vscode](https://code.visualstudio.com/) or other [platformio](https://platformio.org/platformio-ide) compatible IDE you like it, add the platformio plugin insider your IDE (vscode easily provide the plugin by search and install ).

Inside the platformio panel add the below library from the library tab:

Arduino from platform tab
ESP8266 from platforms
SimpleWeather from library
LiquidCrystal_PCF8574 from library
EasyButton from library

Compile the code and upload into your NodeMCU

**To Do**
* Add OTA
* Add plug and play LCD driver address detection
* Increase the functionaity by add more keys 
* OLED have more fun display expriences



