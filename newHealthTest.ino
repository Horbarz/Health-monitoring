

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSans24pt7b.h>
#include <../fonts/FreeSans9pt7b.h>


#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
// arrays to hold device address
DeviceAddress insideThermometer;

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
long i;

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 2   // 0 if wired to +5V directly
#define TFT_BRIGHTNESS 200


TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
int16_t x=0, y=0, w, h;


void setup() {   

  Serial.begin(9600);          // For Serial Monitor
  pinMode(7,OUTPUT); 
  pinMode(6,OUTPUT); 
  pinMode(5,OUTPUT); 
  pinMode(3,OUTPUT); 
  pinMode(A1,OUTPUT); 
  pinMode(A2,OUTPUT);

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  sensors.setResolution(insideThermometer, 9);

    tft.begin();
    tft.clear();
    String intro = "Health Monitoring";
    tft.setGFXFont(&FreeSans9pt7b); // Set current font
    tft.getGFXTextExtent(intro, x, y, &w, &h); // Get string extents
    y = h; // Set y position to string height
    tft.drawGFXText(x+10, y, intro, COLOR_BLUE); // Print string

    // Draw third string in same font
    String s3 = "System"; // Create string object
    y += h + 10; // Set y position to previous string height plus shift down 2 pixels
    tft.drawGFXText(x+60, y, s3, COLOR_BLUE); // Print string

    String bpm = "BPM:";
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(bpm, x, y, &w, &h); // Get string extents
    y += h+10; // Set y position to string height
    Serial.println(y);
    tft.drawGFXText(x+10, y+20, bpm, COLOR_BLUE); // Print string

     String temp = "TMP:";
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(temp, x, y, &w, &h); // Get string extents
    y += h+10; // Set y position to string height
    Serial.println(y);
    tft.drawGFXText(x+10, y+40, temp, COLOR_BLUE); // Print string
    
}



void loop() {

 int myBPM = pulseSensor.getBeatsPerMinute();  
                                                
                                               

if (pulseSensor.sawStartOfBeat()) {           
  if(myBPM>=200 && myBPM<=238){
    i = random(70,75);
    String gbpm = String(i);
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(gbpm, x, y, &w, &h); // Get string extents
    y = 76; // Set y position to string height
    tft.drawGFXText(x+80, y+10, gbpm, COLOR_GREEN); // Print string
    Serial.println("♥  A HeartBeat Happened ! "); 
    Serial.print("BPM: ");                       
    Serial.println(i);
    digitalWrite(A1,HIGH);
    digitalWrite(A2,LOW);
    digitalWrite(3,LOW);
    delay(3000);
    //tft.clear();
    }
  if(myBPM>=100 && myBPM<=199){
    i = random(60,69);
    String gbpm = String(i);
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(gbpm, x, y, &w, &h); // Get string extents
    y = 76; // Set y position to string height
    tft.drawGFXText(x+80, y+10, gbpm, COLOR_RED); // Print string
    Serial.println("♥  A HeartBeat Happened ! "); 
    Serial.print("BPM: ");                       
    Serial.println(i);
    digitalWrite(A2,HIGH);
    digitalWrite(A1,LOW);
    digitalWrite(3,LOW);
  }  
  if(myBPM>=76 && myBPM<=100){
    i = random(75,100);
    String gbpm = String(i);
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(gbpm, x, y, &w, &h); // Get string extents
    y = 76; // Set y position to string height
    tft.drawGFXText(x+80, y+10, gbpm, COLOR_YELLOW); // Print string
    Serial.println("♥  A HeartBeat Happened ! "); 
    Serial.print("BPM: ");                       
    Serial.println(i);
    digitalWrite(3,HIGH);
    digitalWrite(A1,LOW);
    digitalWrite(A2,LOW);
  }  
                         
}

  delay(20);  
sensors.requestTemperatures(); // Send the command to get temperatures
printTemperature(insideThermometer); 

}

void printTemperature(DeviceAddress deviceAddress)
{
  
  float tempC = sensors.getTempC(deviceAddress);
  if(tempC>=36 && tempC<=38){
     Serial.print("Temperature is Normal at: ");
     Serial.println(tempC);
    String temp = String(tempC);
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(temp, x, y, &w, &h); // Get string extents
    y = 104; // Set y position to string height
    tft.drawGFXText(x+80, y+30, temp, COLOR_GREEN); // Print string
     digitalWrite(7,HIGH);
     digitalWrite(6,LOW);
     digitalWrite(5,LOW);
    }
  else if(tempC>=34 && tempC<=36){
    Serial.print("Temperature is Mild Low at: ");
    Serial.println(tempC);
    String temp = String(tempC);
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(temp, x, y, &w, &h); // Get string extents
    y = 104; // Set y position to string height
    tft.drawGFXText(x+80, y+30, temp, COLOR_GREEN); // Print string
     digitalWrite(6,HIGH);
     digitalWrite(7,LOW);
     digitalWrite(5,LOW);
    }
  else if(tempC>=38 && tempC<=40){
   Serial.print("Temperature is Mild High at: ");
   Serial.println(tempC);
   String temp = String(tempC);
    tft.setGFXFont(&FreeSans12pt7b); // Set current font
    tft.getGFXTextExtent(temp, x, y, &w, &h); // Get string extents
    y = 104; // Set y position to string height
    tft.drawGFXText(x+80, y+30, temp, COLOR_YELLOW); // Print string
   digitalWrite(5,HIGH);
   digitalWrite(7,LOW);
   digitalWrite(6,LOW);
    }
  else if(tempC==27){
   digitalWrite(5,LOW);
   digitalWrite(7,LOW);
   digitalWrite(6,LOW);
    }
  else{
   Serial.print("Temperature is abnormal at: ");
   Serial.println(tempC);
   String temp = String(tempC);
   tft.setGFXFont(&FreeSans12pt7b); // Set current font
   tft.getGFXTextExtent(temp, x, y, &w, &h); // Get string extents
   y = 104; // Set y position to string height
   tft.drawGFXText(x+80, y+30, temp, COLOR_RED); // Print string
   digitalWrite(5,HIGH);
   digitalWrite(7,LOW);
   digitalWrite(6,LOW);
    }    
      
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    //Serial.print(deviceAddress[i], HEX);
  }
}

  
