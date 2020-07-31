
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SimpleWeather.h"
#include <Wire.h>              //for ESP8266 use bug free i2c driver https://github.com/enjoyneering/ESP8266-I2C-Driver
#include <LiquidCrystal_PCF8574.h>
#include "mysecret.hpp"
#include <EasyButton.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <neotimer.h>
#include "driver.h"



#define INSIDE_T 1
#define OUTSIDE_C1 2
#define OUTSIDE_C2 3
#define OUTSIDE_F1 4
#define OUTSIDE_F2 5



//#define DEBUG
//#ifndef DEBUG_PRINT
  #ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.println(x)
  #else
    #define DEBUG_PRINT(x)
  #endif
//#endif

#define DHTPIN 14 // D5 GPIO14
#define DHTTYPE DHT22 
// Arduino pin where the buttons are connected to.
#define BUTTON_PIN 0

#define BAUDRATE 115200

static ushort displayT [] = {1,2,3,4,5};
// Instance of the button.
EasyButton button(BUTTON_PIN);
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_PCF8574 lcd(0x3F); // set the LCD address to 0x27 for a 16 chars and 2 line display
weatherData w1,w2;
OpenWeather weather(Key, "Tehran,ir");
OpenWeather forecast(Key, "Tehran,ir",1);

static String Country = "Tehran,Ir";

int shownetwork = 1;


int disSelector = 0;



Neotimer disTimer = Neotimer(10000);
Neotimer wupdateTimer = Neotimer(60000);
struct ambient {
  float Temp;
  float Hmdt;
} ;

ambient room;

ambient readAmbient(int showH=0) //showH=0 dump both humidity and temperature on serial
{
  ambient room = {-1,-1};
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    DEBUG_PRINT(F("Failed to read from DHT sensor!"));
    return room;
  }
  if(showH!=2)
  {  
    if(showH!=1)
    {
      DEBUG_PRINT(F("Humidity: "));
      DEBUG_PRINT(h);
    
    }  

    DEBUG_PRINT(F(",Temperature: "));
    DEBUG_PRINT(t);
    DEBUG_PRINT();
  }  
  room.Temp = t;
  room.Hmdt = h;
  return room;

}

void displayAmbient(ambient room)
{
  lcd.clear();
  lcd.printf("Room T:%.2f ",room.Temp);
  lcd.setCursor(0,1);
  lcd.printf("     H:%.2f ",room.Hmdt);
  
}

void displayWeather(String location,String description,int current=0)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(Country);
  if(current==1)
  {
    lcd.print("    Now");
  }
  else
  {
    lcd.print("    For");  
  }
  
  lcd.setCursor(0,1);
  lcd.print(description);
}

void displayConditions(float Temperature,float Humidity, float Min, float Max)
{
 lcd.clear();                            //Printing Temperature
 lcd.print("T:"); 
 lcd.print(Temperature,1);
 lcd.print((char)223);
 lcd.print("C "); 
                                         
 lcd.print(" H:");                       //Printing Humidity
 lcd.print(Humidity,0);
 lcd.print(" %"); 
 
 lcd.setCursor(0,1);                     //Printing Pressure
 lcd.print("L:");
 lcd.print(Min,1);
 lcd.print("    M:");
 lcd.print(Max,1);
}
void displayGettingData()
{
  lcd.clear();
  delay(100);
  lcd.print("Getting data");
}

void buttonPressed()
{
  shownetwork = 1;

}

void displayNetwork()
{
  lcd.clear();
  lcd.print("IP:");
  lcd.print(WiFi.localIP().toString());
  lcd.setCursor(0,1);
  lcd.print("DNS:");
  lcd.print(WiFi.dnsIP().toString());
}

void sequenceEllapsed()
{
  DEBUG_PRINT("Double click");
}

void buttonISR()
{
  /*
    When button is being used through external interrupts, 
    parameter INTERRUPT must be passed to read() function
   */
  button.read();
}


void setup() {
  // put your setup code here, to run once:
  
  int error = 0;
  Serial.begin(BAUDRATE);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      DEBUG_PRINT("WiFi Failed");
      while(1) {
          delay(1000);
      }
  }
  else
  {
    DEBUG_PRINT("Wifi is connected.");
  }
  
  Wire.begin();
  Wire.beginTransmission(0x3F);
  error = Wire.endTransmission();
  if (error == 0) {
    DEBUG_PRINT("LCD found.");
    lcd.begin(16, 2); // initialize the lcd
    lcd.setBacklight(255);
    lcd.clear();
    delay(100);
    lcd.print("Connected");
  } else {
    DEBUG_PRINT("LCD not found.");
  
  } 
  button.begin();
  button.onPressed(buttonPressed);
  button.onSequence(2, 1500, sequenceEllapsed);
  if (button.supportsInterrupt())
  {
    button.enableInterrupt(buttonISR);
    DEBUG_PRINT("Button will be used through interrupts");
  }
  else
  {
    DEBUG_PRINT("Button is starting without interrupt");
  }
  dht.begin();
  DEBUG_PRINT("DHT is starting.");
  DEBUG_PRINT("Setting up the timers.");
  disTimer.set(10000);
  wupdateTimer.set(60000);
  lcd.setCursor(0,1);
  delay(400);
  lcd.print("Getting data");
  unsigned long oldmilis = millis();
  weather.updateStatus(&w1);
  forecast.updateStatus(&w2);
  lcd.clear();
  lcd.printf("Take:%ul msec",(millis()-oldmilis));
  delay(4000);
}

void loop() {
  

  if(shownetwork==1)
  {
    displayNetwork();
    shownetwork = 0;
  }
  if (wupdateTimer.repeat())
  {
    displayGettingData();
    weather.updateStatus(&w1);
    forecast.updateStatus(&w2);


  }
  if (disTimer.repeat())
  {
    int disSize = *(&displayT + 1) - displayT;
    if (disSelector<disSize)
    {
      switch(displayT[disSelector])
      {
        case INSIDE_T:
          
          room = readAmbient(2);
          displayAmbient(room);



          break;
        case OUTSIDE_C1:
          displayWeather( w1.weather, w1.description,1);
          break;

        case OUTSIDE_C2:
          displayConditions(w1.current_Temp, w1.humidity,w1.min_temp,w1.max_temp);
          break;

        case OUTSIDE_F1:          
          displayWeather( w2.weather, w2.description);
          break;

        case OUTSIDE_F2:
          displayConditions(w2.current_Temp, w2.humidity,w2.min_temp,w2.max_temp);
          break;



      }
      disSelector++;
    }
    else
    {
      room = readAmbient(2);
      displayAmbient(room);  
      weather.updateStatus(&w1);
      forecast.updateStatus(&w2);
      disSelector = 1;    
    }
    
  }
  
}

