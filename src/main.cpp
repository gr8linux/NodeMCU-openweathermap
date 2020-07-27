
#define DEBUG_PRINT
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SimpleWeather.h"
#include <Wire.h>              //for ESP8266 use bug free i2c driver https://github.com/enjoyneering/ESP8266-I2C-Driver
#include <LiquidCrystal_PCF8574.h>
#include "mysecret.h"
#include <EasyButton.h>

// Arduino pin where the buttons are connected to.
#define BUTTON_PIN 0

#define BAUDRATE 9600

// Instance of the button.
EasyButton button(BUTTON_PIN);


LiquidCrystal_PCF8574 lcd(0x3F); // set the LCD address to 0x27 for a 16 chars and 2 line display
weatherData w;
OpenWeather weather(Key, "Tehran,ir");
OpenWeather forecast(Key, "Tehran,ir",1);

String Country = "Tehran, Ir";


void displayWeather(String location,String description)
{
  lcd.clear();
  lcd.setCursor(0,0);
  //lcd.print(location);
  //lcd.print(", ");
  lcd.print(Country);
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
  lcd.print("Getting data");
}

void buttonPressed()
{
  lcd.clear();
  //lcd.setCursor(0,0);
  lcd.print("IP:");
  //lcd.setCursor(0,1);
  lcd.print(WiFi.localIP().toString());
  lcd.setCursor(0,1);
  lcd.print("DNS:");
  lcd.print(WiFi.dnsIP().toString());
  //lcd.blink();
  //delay(1000);  

  //Serial.println("Button pressed");
}


void sequenceEllapsed()
{
  Serial.println("Double click");
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
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed");
      while(1) {
          delay(1000);
      }
  }
  Wire.begin();
  Wire.beginTransmission(0x3F);
  error = Wire.endTransmission();
  if (error == 0) {
    Serial.println("LCD found.");
    lcd.begin(16, 2); // initialize the lcd
    lcd.setBacklight(255);

  } else {
    Serial.println("LCD not found.");
  } 
  
  button.begin();

  button.onPressed(buttonPressed);

  button.onSequence(2, 1500, sequenceEllapsed);

  if (button.supportsInterrupt())
  {
    button.enableInterrupt(buttonISR);
    Serial.println("Button will be used through interrupts");
  }
  else
  {
    Serial1.println("Button is starting without interrupt");
  }
  

}

void loop() {
  Serial.println("\nOpenWeather Current:");
  weather.updateStatus(&w);
  Serial.print("Weather: ");
  Serial.println(w.weather);
  Serial.print("Description: ");
  Serial.println(w.description);
  Serial.print("ID: ");
  Serial.println(w.id);
  Serial.print("Current Temp: ");
  Serial.println(w.current_Temp);
  Serial.print("Min Temp: ");
  Serial.println(w.min_temp);
  Serial.print("Max Temp: ");
  Serial.println(w.max_temp);
  Serial.print("Humidity: ");
  Serial.println(w.humidity);
  Serial.print("Rain: ");
  Serial.println(w.rain);
  displayWeather( w.weather, w.description);
  delay(10000);
  displayConditions(w.current_Temp, w.humidity,w.min_temp,w.max_temp);
  //Serial.print("Full Response: ");
  //Serial.println(weather.getResponse().c_str());

  Serial.println("\nOpenWeather Forecast:");
  forecast.updateStatus(&w);
  Serial.print("Weather: ");
  Serial.println(w.weather);
  Serial.print("Description: ");
  Serial.println(w.description);
  Serial.print("ID: ");
  Serial.println(w.id);
  Serial.print("Current Temp: ");
  Serial.println(w.current_Temp);
  Serial.print("Min Temp: ");
  Serial.println(w.min_temp);
  Serial.print("Max Temp: ");
  Serial.println(w.max_temp);
  Serial.print("Humidity: ");
  Serial.println(w.humidity);
  Serial.print("Rain: ");
  Serial.println(w.rain);
  //Serial.print("Full Response: ");
  //Serial.println(forecast.getResponse().c_str());

  displayWeather( w.weather, w.description);
  delay(10000);
  displayConditions(w.current_Temp, w.humidity,w.min_temp,w.max_temp);

  //delay(10000);       // Wait for 600 seconds
  // put your main code here, to run repeatedly:
}

