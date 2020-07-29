**NodeMCU openweathermap** 

![live picture](./lib/live.png?raw=true "Live Image")

This project read the weather information from <a href="https://openweathermap.org/" target="_blank">openweathermap</a> and disply on 2x16 LCD.
Please remember to modify the project you need to edit the code inside the platformio and compile it.

**Getting Start**


**Software**

Install [vscode](https://code.visualstudio.com/) or other [platformio](https://platformio.org/platformio-ide) compatible IDE you like it, add the platformio plugin insider your IDE (vscode easily provide the plugin by search and install ).

Inside the platformio panel add the below library from the library tab:

Arduino from platform tab
ESP8266 from platforms
SimpleWeather from library
LiquidCrystal_PCF8574 from library
EasyButton from library
Edit mysecret.hpp with your own openweathermap key and your wifi secret and SSID
Compile the code and upload into your NodeMCU

**Hardware**

* NodeMCU board mine have 4M flash 
* 16x2 LCD with i2c pcf8574 ( you just used 2 GPIO to drive the LCD)
* Single row female cable
* USB phone charger or computer USB 
* microusb cable


**Pinout**

For NodeMCU you may connect the wire like the below 
![Pinout](./lib/pinout.png?raw=true "Pinout")

  I2C LCD Module<------------>ESP8266

* GND   <--------------------> GND
* VCC    <-------------------> Vin
* SDA   <--------------------> D2 (NodeMCU) GPIO 4
* SCL   <--------------------> D1 (NodeMCU) GPIO 5

**Deep dive**
Openweathermap will prepare some level of free weather API to fetch the current and forcast information by calling the REST API.
You need to subscribe and get your application KEY and update your mysecret.hpp file with that information.
NodeMCU will connect to the internet via WiFi and call the API each minute and display the information on 2X16 LCD.
I add some hack in User button built in with NodeMCU to display the IP address and DNS just in case you can't see the result you will sure that the network is available.
I


**To Do**
* Implement nonblocking delays
* Connect to the cloud or an MQTT service
* OTA update 
* *Reading the indoor temperature and humidity by DHT22* Done
* HTTP server to show weather information and configuration on the web
* Add plug and play LCD driver address detection it's made me fool some times to find it
* Increase the functionaity by add more keys to have some access to configuration 
* OLED have more fun display expriences 
* Hacking some graphic character to make UI more readable
  



