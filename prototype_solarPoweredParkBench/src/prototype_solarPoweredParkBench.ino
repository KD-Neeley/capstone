/*
 * Project prototype_solarPoweredParkBench
 * Description: The Solar Powered Park Bench will provide visitors with the ability to
 *              charge their devices using a USB port. The lights on the bench will
 *              provide mild entertainment by responding to sounds in the environment.
 * 
 *              * Get sound detection using the MSGEQ7 sound detector 
 *              Make NeoPixels respond to the sound input
 *              Detect the presence of someone sitting on the bench 
 *              Put the program in sleep mode when no presence is detected
 *              Wake the program and run it when a presence is detected
 *              * Power the microcontroller using a Solar Panel 
 *              Charge a 12v battery using the Solar Panel
 *              Switch power to battery operation when the sun is gone
 *              * Provide a charge to a USB port to charge common devices (Phones, Ipads, etc)
 *              * Get the train and bus schedules for the installation site
 *              * Display the train and bus schedules on the OLED
 *              * Display the current time and date on the OLED
 *              * Display the current temperature on the OLED
 *              * Publish the data to a dashboard
 * 
 * Author: Katie Neeley
 * Date: 04/11/2023
 */
#include "Particle.h"
#include <neopixel.h>
#include "kdsRainbows.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <JsonParserGeneratorRK.h>
#include "credentials.h"




//GLOBALS
//MSGEQ7 Sound Sensor
int const STROBE = A3;
int const OUT = A4;
int const RESETPIN = A2;
int soundInput;
int soundLevel[7];
int pulse;


//Neopixels
int const PIXELCOUNT= 16;
int const PIXELPIN = D8;
int startPixel;
int endPixel;
int hexColor;
int brightness;
int showTime;

//OLED
const int OLED_RESET = D4;

//Distance Sensor
const int MOTIONSENSOR = A1;
int distance;
int distanceThreshold = 2900;

//QR Code for Dashboard
// 'QRcode', 128x64px
// 'QRcode', 128x64px
const unsigned char QRcode [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x60, 0x78, 0x18, 0x06, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x60, 0x78, 0x18, 0x06, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x66, 0x61, 0xe0, 0x66, 0x7f, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x66, 0x61, 0xe0, 0x66, 0x7f, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x7e, 0x67, 0xe7, 0xfe, 0x60, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x7e, 0x67, 0xe7, 0xfe, 0x60, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x67, 0x9e, 0x19, 0xfe, 0x60, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x67, 0x9e, 0x19, 0xfe, 0x60, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x79, 0xfe, 0x61, 0x9e, 0x60, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x79, 0xfe, 0x61, 0x9e, 0x60, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x7f, 0x99, 0x86, 0x1e, 0x7f, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x7f, 0x99, 0x86, 0x1e, 0x7f, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x66, 0x66, 0x66, 0x66, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x66, 0x66, 0x66, 0x66, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xe7, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xe7, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x18, 0x19, 0xf9, 0x98, 0x1e, 0x79, 0x87, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x18, 0x19, 0xf9, 0x98, 0x1e, 0x79, 0x87, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x61, 0x87, 0xf8, 0x1e, 0x00, 0x07, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x61, 0x87, 0xf8, 0x1e, 0x00, 0x07, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x18, 0x1e, 0x61, 0xe1, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x18, 0x1e, 0x61, 0xe1, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xe7, 0xff, 0xe6, 0x07, 0x9f, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xe7, 0xff, 0xe6, 0x07, 0x9f, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x66, 0x79, 0x86, 0x19, 0xe7, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x66, 0x79, 0x86, 0x19, 0xe7, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf9, 0xe1, 0xe0, 0x66, 0x60, 0x1e, 0x7e, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf9, 0xe1, 0xe0, 0x66, 0x60, 0x1e, 0x7e, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x66, 0x7f, 0x81, 0x87, 0x98, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x66, 0x7f, 0x81, 0x87, 0x98, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf9, 0xe7, 0x81, 0x86, 0x66, 0x1f, 0x87, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf9, 0xe7, 0x81, 0x86, 0x66, 0x1f, 0x87, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x1e, 0x61, 0x86, 0x1e, 0x61, 0xe1, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x1e, 0x61, 0x86, 0x1e, 0x61, 0xe1, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf9, 0xf9, 0x99, 0xfe, 0x66, 0x67, 0x98, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf9, 0xf9, 0x99, 0xfe, 0x66, 0x67, 0x98, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x00, 0x00, 0x06, 0x7e, 0x66, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x00, 0x00, 0x06, 0x7e, 0x66, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x67, 0xff, 0x80, 0x01, 0x9e, 0x66, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x67, 0xff, 0x80, 0x01, 0x9e, 0x66, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x60, 0x60, 0x18, 0x00, 0x60, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x60, 0x60, 0x18, 0x00, 0x60, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x67, 0xff, 0xe7, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x67, 0xff, 0xe7, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x67, 0xe6, 0x67, 0x86, 0x61, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x67, 0xe6, 0x67, 0x86, 0x61, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x66, 0x18, 0x1e, 0x07, 0xe1, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x66, 0x18, 0x1e, 0x07, 0xe1, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x78, 0x7f, 0xf8, 0x60, 0x06, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x78, 0x7f, 0xf8, 0x60, 0x06, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x61, 0xe7, 0x87, 0xe1, 0x81, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x61, 0xe7, 0x87, 0xe1, 0x81, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x61, 0x99, 0xf8, 0x07, 0x9e, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe6, 0x06, 0x61, 0x99, 0xf8, 0x07, 0x9e, 0x67, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x79, 0x80, 0x1e, 0x61, 0x99, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x79, 0x80, 0x1e, 0x61, 0x99, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x66, 0x7f, 0x9e, 0x00, 0x7f, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x66, 0x7f, 0x9e, 0x00, 0x7f, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


//WEBHOOK GLOBALS
const char *EVENT_NAME = "GetWeatherData";
unsigned int lastTime;
const float lat=35.0045, lon=-106.6465;

//function prototype
int pixelFill (int startPixel, int endPixel, int brightness, int hexColor);
// int tradRainbow (int startPixel, int endPixel, int hexColor);
// int movingTradRainbow (int startPixel, int endPixel, int hexColor);

//Objects
Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, WS2812B);
Adafruit_SSD1306 myDisplay(OLED_RESET);


void rainBow(uint8_t holdOn);
uint32_t Wheel(byte WheelPos);


SYSTEM_MODE(AUTOMATIC);

void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 15000);
    String subscriptionName = String::format("%s/%s/", System.deviceID().c_str(), EVENT_NAME);
    Particle.subscribe(subscriptionName, subscriptionHandler, MY_DEVICES);
    Serial.printf("Subscribing to %s\n", subscriptionName.c_str());
    pixel.begin();
 

    //Connect to Internet but not Particle Cloud
    WiFi.on();
    WiFi.connect();
    while(WiFi.connecting()) {
        Serial.printf(".");
    }
    Serial.printf("\n\n");


    //initialize Sound Input
   // Define our pin modes
    pinMode(STROBE, OUTPUT);
    pinMode(RESETPIN, OUTPUT);
    pinMode(OUT, INPUT);
 
// Initialize the Sound Sensor Pins
    digitalWrite(RESETPIN, LOW);
    digitalWrite(STROBE, LOW);
    delay(1);
 
  // Reset MSGEQ7 
    digitalWrite(RESETPIN, HIGH);
    delayMicroseconds(.1);
    digitalWrite(RESETPIN, LOW);
    digitalWrite(STROBE, HIGH); 
    delayMicroseconds(72);

//Initialize the OLED
    myDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    myDisplay.clearDisplay();

//Initialize the IR Distance Sensor
    pinMode(MOTIONSENSOR, INPUT);
}

void loop() {
    distance = analogRead(MOTIONSENSOR); //Get Distance to determine if the program will run given a presence
    Serial.printf("Distance Read is %i\n", distance);

    if(distance > distanceThreshold) { //if a presence is sensed run the program
        // if((millis()-lastTime) > 60000) {
        //     Serial.printf("\n\nTime = %i\n", millis());
        //     Particle.publish(EVENT_NAME, "", PRIVATE);
        //     Particle.publish(EVENT_NAME, String::format("{\"lat\":%0.5f,\"lon\":%0.5f}", lat, lon), PRIVATE);
        //     lastTime = millis();
        // }
        //Starting Light       
        pixelFill(0, 16, 255, fullyellow);
        pixel.show();
        // delay(15000);
           
        //Sample Data
        myDisplay.clearDisplay();
        myDisplay.display();
        myDisplay.setRotation(2);
        myDisplay.setCursor (1,1);
        myDisplay.setTextSize(2);
        myDisplay.setTextColor(WHITE);
        myDisplay.printf("04/19/2023\n05:02P 68F\nLOW: 52F  HIGH: 72F\n");
        myDisplay.display();
        delay(3000);

        //Sample Data
        myDisplay.clearDisplay();
        myDisplay.display();
        myDisplay.setRotation(2);
        myDisplay.setCursor (1,1);
        myDisplay.setTextSize(1);
        myDisplay.setTextColor(WHITE);
        myDisplay.printf("ETA\n05:45P : Redline\n06:45 : #66\n06:00P Rail Runner\n");
        myDisplay.display();
        delay(3000);

        //Sample Data
        myDisplay.clearDisplay();
        myDisplay.display();
        myDisplay.setRotation(2);
        myDisplay.drawBitmap(0, 0, QRcode, 128, 64, WHITE);;
        myDisplay.display();
        delay(3000);

        //GET Sound
        // Cycle through each frequency band by pulsing the strobe.
        //Sound Level 0
        digitalWrite(STROBE, LOW);
        delayMicroseconds(72);
        soundLevel[0] = analogRead(OUT);
        Serial.printf("Level 0\n Value %i\n", soundLevel[0]);
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(72); 
        if(soundLevel[0] < 400) {
            pixelFill(0, 2, 50, fullred);
        }
        if(soundLevel[0] >= 400 && soundLevel[0] < 1000) {
            pixelFill(0, 2, 100, fullred);
        }
        if(soundLevel[0] >= 1000 && soundLevel[0] < 2000) {
            pixelFill(0, 2, 150, fullred);
        }
        if(soundLevel[0] >= 2000 && soundLevel[0] < 3000) {
            pixelFill(0, 2, 200, fullred);
        }
        if(soundLevel[0] >= 3000) {
            pixelFill(0, 2, 255, fullred);
        }

        //Sound Level 1
        digitalWrite(STROBE, LOW);
        delayMicroseconds(72);
        soundLevel[1] = analogRead(OUT);
        Serial.printf("Level 1\n Value %i\n", soundLevel[1]);
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(72); 
        if(soundLevel[1] < 400) {
            pixelFill(3, 5, 50, fullmagenta);
        }
        if(soundLevel[1] >= 400 && soundLevel[1] < 1000) {
            pixelFill(3, 5, 100, fullmagenta);
        }
        if(soundLevel[1] >= 1000 && soundLevel[1] < 2000) {
            pixelFill(3, 5, 150, fullmagenta);
        }
        if(soundLevel[1] >= 2000 && soundLevel[1] < 3000) {
            pixelFill(3, 5, 200, fullmagenta);
        }
        if(soundLevel[1] >= 3000) {
            pixelFill(3, 5, 255, fullmagenta);
        }
         //Sound Level 2
        digitalWrite(STROBE, LOW);
        delayMicroseconds(72);
        soundLevel[2] = analogRead(OUT);
        Serial.printf("Level 2\n Value %i\n", soundLevel[2]);
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(72); 
        if(soundLevel[2] < 400) {
            pixelFill(6, 8, 50, purple);
        }
        if(soundLevel[2] >= 400 && soundLevel[2] < 1000) {
            pixelFill(6, 8, 100, purple);
        }
        if(soundLevel[2] >= 1000 && soundLevel[2] < 2000) {
            pixelFill(6, 8, 150, purple);
        }
        if(soundLevel[2] >= 2000 && soundLevel[2] < 3000) {
            pixelFill(6, 8, 200, purple);
        }
        if(soundLevel[2] >= 3000) {
            pixelFill(6, 8, 255, purple);
        }
        //Sound Level 3
        digitalWrite(STROBE, LOW);
        delayMicroseconds(72);
        soundLevel[3] = analogRead(OUT);
        Serial.printf("Level 3\n Value %i\n", soundLevel[3]);
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(72); 
        if(soundLevel[3] < 400) {
            pixelFill(9, 11, 50, fullblue);
        }
        if(soundLevel[3] >= 400 && soundLevel[3] < 1000) {
            pixelFill(9, 11, 100, fullblue);
        }
        if(soundLevel[3] >= 1000 && soundLevel[3] < 2000) {
            pixelFill(9, 11, 150, fullblue);
        }
        if(soundLevel[3] >= 2000 && soundLevel[3] < 3000) {
            pixelFill(9, 11, 200, fullblue);
        }
        if(soundLevel[3] >= 3000) {
            pixelFill(9, 11, 255, fullblue);
        }
        //Sound Level 4
        digitalWrite(STROBE, LOW);
        delayMicroseconds(72);
        soundLevel[4] = analogRead(OUT);
        Serial.printf("Level 4\n Value %i\n", soundLevel[4]);
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(72); 
        if(soundLevel[4] < 400) {
            pixelFill(12, 13, 50, fullgreen);
        }
        if(soundLevel[4] >= 400 && soundLevel[4] < 1000) {
            pixelFill(12, 13, 100, fullgreen);
        }
        if(soundLevel[4] >= 1000 && soundLevel[4] < 2000) {
            pixelFill(12, 13, 150, fullgreen);
        }
        if(soundLevel[4] >= 2000 && soundLevel[4] < 3000) {
            pixelFill(12, 13, 200, fullgreen);
        }
        if(soundLevel[4] >= 3000) {
            pixelFill(12, 13, 255, fullgreen);
        }
        //Sound Level 5
        digitalWrite(STROBE, LOW);
        delayMicroseconds(72);
        soundLevel[5] = analogRead(OUT);
        Serial.printf("Level 5\n Value %i\n", soundLevel[5]);
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(72); 
        if(soundLevel[4] < 400) {
            pixelFill(14, 15, 50, fullyellow);
        }
        if(soundLevel[5] >= 400 && soundLevel[5] < 1000) {
            pixelFill(14, 15, 100, fullyellow);
        }
        if(soundLevel[5] >= 1000 && soundLevel[5] < 2000) {
            pixelFill(14, 15, 150, fullyellow);
        }
        if(soundLevel[5] >= 2000 && soundLevel[5] < 3000) {
            pixelFill(14, 15, 200, fullyellow);
        }
        if(soundLevel[5] >= 3000) {
            pixelFill(14, 15, 255, fullyellow);
        }
        //Sound Level 6
        digitalWrite(STROBE, LOW);
        delayMicroseconds(72);
        soundLevel[6] = analogRead(OUT);
        Serial.printf("Level 6\n Value %i\n", soundLevel[6]);
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(72); 
        if(soundLevel[6] < 400) {
            pixelFill(14, 15, 50, orange);
        }
        if(soundLevel[6] >= 400 && soundLevel[6] < 1000) {
            pixelFill(16, 16, 100, orange);
        }
        if(soundLevel[6] >= 1000 && soundLevel[6] < 2000) {
            pixelFill(16, 16, 150, orange);
        }
        if(soundLevel[6] >= 2000 && soundLevel[6] < 3000) {
            pixelFill(16, 16, 200, orange);
        }
        if(soundLevel[6] >= 3000) {
            pixelFill(16, 16, 255, orange);
        } 
        //sampleLighting
        rainBow(20);
       
    }
    if(distance < distanceThreshold) {
            distance = analogRead(MOTIONSENSOR);
            myDisplay.clearDisplay();
            myDisplay.display();
            pixel.clear();
            pixel.show();
    } 

    
   
}
////FUNCTION DEFINITIONS


void rainBow(uint8_t holdOn) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixel.numPixels(); i++) {
      pixel.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixel.show();
    delay(holdOn);
  }
}
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

//WEBHOOK 
void subscriptionHandler(const char *event, const char *data) {

    JSONValue outerObj = JSONValue::parseCopy(data);
    JSONObjectIterator iter(outerObj);
    while(iter.next()){
        Serial.printf("key=%s value=%s\n", (const char*) iter.name(), (const char*)
        iter.value().toString());
        myDisplay.clearDisplay();
        myDisplay.display();
        myDisplay.setRotation(2);
        myDisplay.setCursor (1,1);
        myDisplay.setTextSize(2);
        myDisplay.setTextColor(WHITE);
        myDisplay.printf("%s %s\n", (const char*) iter.name(), (const char*) iter.value().toString());
        myDisplay.display();
    }
}

//NEOPIXEL FUNCTIONS
int pixelFill(int startPixel, int endPixel, int brightness, int hexColor) {
            // pixel.clear();
            // pixel.show();
            pixel.setBrightness(brightness);
            for(int i = startPixel; i < endPixel; i++) {
                pixel.setPixelColor(i, hexColor);
            }
            pixel.show();
            delay(100);
            // pixel.clear();
            // pixel.show();

            return(endPixel);
}

// int tradRainbow (int startPixel, int endPixel, int hexColor) {
//     int maxBrightness = 255;
//     int minBrightness = 20;
//     int startColor = 0;
//     int endColor = 9;

//     for(int b=minBrightness; b<=maxBrightness; b++) {
//         brightness = b;
//         pixel.setBrightness(b);
//         for(int c = startColor; c <= endColor; c++) {
//             hexColor = traditionalrainbow[c];
//             for(int i=startPixel; i<endPixel+1; i++) {
//                     pixel.setPixelColor(i, hexColor);
//                     pixel.show(); 
//                 }
            
//             delay(100);
//         }
//     }
//     for(int b = maxBrightness; b >= minBrightness; b--) {
//         pixel.setBrightness(b);
//         for(int c = endColor; c >= startColor; c--) {
//             hexColor = traditionalrainbow[c];
//             for(int i=startPixel; i<endPixel+1; i++) {
//                     pixel.setPixelColor(i, hexColor);
//                     pixel.show(); 
//                 }
//             delay(100);
//         }
//     }
//     pixel.show();
//     return(endPixel);
// }

// int movingTradRainbow (int startPixel, int endPixel, int hexColor) {
//     int maxBrightness = 255;
//     int minBrightness = 20;
//     int startColor = 0;
//     int endColor = 9;

//     for(int b=minBrightness; b<= maxBrightness; b++) {
//         pixel.setBrightness(b);
//         Serial.printf("Brightness = %i \n", b);
//         for(int i=startPixel; i<endPixel+1; i++) {
//             for(int c = startColor; c <= endColor; c++) {
//                 hexColor = traditionalrainbow[c];
//                         pixel.setPixelColor(i, hexColor);
//                         pixel.show(); 
//                         delay(15);
//                         pixel.clear();
//                         pixel.show();
//                 }
          
//         }

//     }
//     for(int b = maxBrightness; b >= minBrightness; b--) {
//         pixel.setBrightness(b);
//         for(int i = endPixel; i>= startPixel; i--) {
//             for(int c = endColor; c >= startColor; c--) {
//                 hexColor = traditionalrainbow[c];
//                         pixel.setPixelColor(i, hexColor);
//                         pixel.show(); 
//                         delay(15);
//                         pixel.clear();
//                         pixel.show();
//             }
            
//         }
//     }
//     pixel.show();
//     return (endPixel);
// }

//SAMPLE CODE:

//in void Loop:
    // if((millis()-lastTime>10000)) {
    //         if(mqtt.Update()) {
    //             // randLat = random(35.092891, 35.245520);
    //             randLat = 35.000000+(random(0.000000, 1000000)/1000000.000000);
    //             // randLon = random(-106.648240, -106.610122);
    //             randLon = -106.000000+(random(0.000000, 1000000)/100000.000000);
    //             Serial.printf("Lat = %0.6f , Lon = %0.6f\n", randLat, randLon);
    //             createEventPayLoad(randLat, randLon);
    //             // gpsFeed.publish(lat, lon); //publishing via JSON, you don't need this line
    //             Serial.printf("Publishing %0.6f, %0.6f\n", randLat, randLon);

    //         }
    //         lastTime = millis();
    //     }
//Funciton: createEventPayLoad
// void createEventPayLoad(float lat, float lon) {
//     JsonWriterStatic<256> jw; 
//     {
//         JsonWriterAutoObject obj(&jw);

//         jw.insertKeyValue("lat", randLat);
//         jw.insertKeyValue("lon", randLon);
//     }
//     gpsFeed.publish(jw.getBuffer());
// }

//OLD CODE
 
        //             //Light Iteration Variables for Neopixel settings
        //             static int upDown = 1;
        //             static int pixelNum = 0;
        //             static int lastPixelTime = 0;
        //             int maxPixel = 16;
        //             int minPixel = 0;
                    

        //             if(millis()-lastPixelTime > 500) {  //start timer
        //                 lastPixelTime = millis();       //reset timer
        //                 pixelNum = pixelNum + upDown;   //iterate through NeoPixels
        //                 for (int i = 0; i < 7; i++) {
        //                     digitalWrite(STROBE, LOW);
        //                     delayMicroseconds(100);                  
        //                     soundLevel[i] = analogRead(OUT);
        //                     digitalWrite(STROBE, HIGH);
        //                     delayMicroseconds(1);                   
        //                     }
                            
        //                     for (int i = 0; i < 7; i++) {
        //                         Serial.printf("%i", soundLevel[i]);
        //                     }
        // }

        //             //         if(soundLevel[pulse]>=0 && soundLevel[pulse]<63) {
        //             //             if((pixelNum==maxPixel)||(pixelNum==minPixel)) {
        //             //             upDown = -1*upDown; //multiply by -1 to iterate up and down the nepixel lights
        //             //             }
                        
        //             //             pixel.setBrightness(50);
        //             //             pixel.clear();
        //             //             pixel.setPixelColor(pixelNum, fullblue);
        //             //             pixel.show();

        //             //             digitalWrite(STROBE, LOW);
        //             //             digitalWrite(RESET, HIGH);
        //             //             soundLevel[pulse] = analogRead(OUT);    //Get Sound
        //             //         }
        //             //         if(soundLevel[pulse]>=63 && soundLevel[pulse]<160) {
        //             //             if((pixelNum==maxPixel)||(pixelNum==minPixel)) {
        //             //             upDown = -1*upDown; //multiply by -1 to iterate up and down the nepixel lights
        //             //             }
                               
        //             //             pixel.setBrightness(50);
        //             //             pixel.clear();
        //             //             pixel.setPixelColor(pixelNum, fullgreen);
        //             //             pixel.show();

        //             //             digitalWrite(STROBE, LOW);
        //             //             digitalWrite(RESET, HIGH);
        //             //             soundLevel[pulse] = analogRead(OUT);    //Get Sound

        //             //         }
        //             //         if(soundLevel[pulse]>=160 && soundLevel[pulse]<400) {
        //             //             if((pixelNum==maxPixel)||(pixelNum==minPixel)) {
        //             //             upDown = -1*upDown; //multiply by -1 to iterate up and down the nepixel lights
        //             //             }
                                
        //             //             pixel.setBrightness(75);
        //             //             pixel.clear();
        //             //             pixel.setPixelColor(pixelNum, fullyellow);
        //             //             pixel.show();

        //             //             digitalWrite(STROBE, LOW);
        //             //             digitalWrite(RESET, HIGH);
        //             //             soundLevel[pulse] = analogRead(OUT);    //Get Sound

        //             //         }
        //             //         if(soundLevel[pulse]>=400 && soundLevel[pulse]<1000) {
        //             //             if((pixelNum==maxPixel)||(pixelNum==minPixel)) {
        //             //             upDown = -1*upDown; //multiply by -1 to iterate up and down the nepixel lights
        //             //             }
                                
        //             //             pixel.setBrightness(100);
        //             //             pixel.clear();
        //             //             pixel.setPixelColor(pixelNum, orange);
        //             //             pixel.show();

        //             //             digitalWrite(STROBE, LOW);
        //             //             digitalWrite(RESET, HIGH);
        //             //             soundLevel[pulse] = analogRead(OUT);    //Get Sound

        //             //         }
        //             //         if(soundLevel[pulse]>=1000 && soundLevel[pulse]<2500) {
        //             //             if((pixelNum==maxPixel)||(pixelNum==minPixel)) {
        //             //             upDown = -1*upDown; //multiply by -1 to iterate up and down the nepixel lights
        //             //             }
                        
        //             //             pixel.setBrightness(150);
        //             //             pixel.clear();
        //             //             pixel.setPixelColor(pixelNum, fullred);
        //             //             pixel.show();

        //             //             digitalWrite(STROBE, LOW);
        //             //             digitalWrite(RESET, HIGH);
        //             //             soundLevel[pulse] = analogRead(OUT);    //Get Sound

        //             //         }
        //             //         if(soundLevel[pulse]>=2500 && soundLevel[pulse]<6250) {
        //             //             if((pixelNum==maxPixel)||(pixelNum==minPixel)) {
        //             //             upDown = -1*upDown; //multiply by -1 to iterate up and down the nepixel lights
        //             //             }
                                
        //             //             pixel.setBrightness(200);
        //             //             pixel.clear();
        //             //             pixel.setPixelColor(pixelNum, fullmagenta);
        //             //             pixel.show();

        //             //             digitalWrite(STROBE, LOW);
        //             //             digitalWrite(RESET, HIGH);
        //             //             soundLevel[pulse] = analogRead(OUT);    //Get Sound

        //             //         }
        //             //         if(soundLevel[pulse]>=6250 && soundLevel[pulse]<16000) {
        //             //             if((pixelNum==maxPixel)||(pixelNum==minPixel)) {
        //             //             upDown = -1*upDown; //multiply by -1 to iterate up and down the nepixel lights
        //             //             }
                                
        //             //             pixel.setBrightness(255);
        //             //             pixel.clear();
        //             //             pixel.setPixelColor(pixelNum, purple);
        //             //             pixel.show();

        //             //             digitalWrite(STROBE, LOW);
        //             //             digitalWrite(RESET, HIGH);
        //             //             soundLevel[pulse] = analogRead(OUT);    //Get Sound

        //             //         }
        //             //     }
                        
        //             // }
        //             distance = analogRead(MOTIONSENSOR);
        // if(distance<3000) { //If no presence is sensed, just get the distance reading until a presence is sensed
        //     distance = analogRead(MOTIONSENSOR);
        // }


//ATTEMPT V2.0
// #include <HttpClient.h>
// #include "credentials.h"
// //JSON Xfer
// int const PORT=443; //80 is normal and secure, 443 is unsecure
// const char* HOST = "https://gluebench.bubbleapps.io";
// int nextTime;
// int lastTime;
// int mainTemp;
// // int lowTemp;
// // int highTemp;
// // String weather;

// //Objects for JSON Xfer
// // TCPClient TheClient; // Create TCP Client object                       
// // byte server[] = { 104, 19, 217, 48}; //https://gluebench.bubbleapps.io/
// HttpClient http;
// http_request_t request;
// http_response_t response;
// JsonParserStatic<1024, 20> jsonParser;
// // http_header_t headers[] = {
// //     {"Authorization", String("Bearer " + String(getWeather)).c_str()}
// // };

 // //Get the Temperature data from Bubble
    //    if(WiFi.connecting() == false) {
    //         request.hostname = HOST;
    //         request.port = PORT;
    //         request.path = "/api/1.1/wf/weatherreport";
    //         // request.headers = "Authorization: Bearer " + String(getWeather) + "\r\n";
    //         http.get(request, response);
    //         // http.get(request, response, headers, sizeof(headers) / sizeof(headers[0]));

    //         if (response.status == 200) {
    //              Serial.println(response.body);

    //             const char* json = response.body.c_str();
                
    //             // JsonParserGeneratorRK::jsmntok_t tokens[32];
    //             // int tokenCount = sizeof(tokens) / sizeof(tokens[0]);
    //             int result = jsonParser.parse();

    //             if (result >= 0) {
    //                 float mainTemp = jsonParser.parse();
    //                 // float lowTemp = jsonParser.parse();
    //                 // float highTemp = jsonParser.parse();
    //                 // const char* weatherReport = jsonParser.parse();
    //                 // const char* date = jsonParser.parse();

    //                 Serial.printf("Main Temp: %i\n", mainTemp);
    //                 // Serial.printf("Low Temp: %i\n", lowTemp);
    //                 // Serial.printf("High Temp: %i \n", highTemp);
    //                 // Serial.printf("Weather Report: %s\n", weatherReport);
    //                 // Serial.printf("Date: %s\n", date);
    //             }
    //             else {
    //                 Serial.println("Failed to parse JSON");
    //             }
    //         }
    //         else {
    //         Serial.print("HTTP GET request failed, error: ");
    //         Serial.println(response.status);
    //         }

    //         delay(5000);
    //     }


           //ATTEMPT V1.0
  
        //JSON Get and Post Globals, MQTT Globals
// float mainTemp;
// unsigned int last, lastTime;
// const char *EVENT_NAME = "getMainTemp";    
// byte dataBuffer[1024];
// String receivedData;
// void callback(char* topic, byte* payload, unsigned int length);
// //MQTT Object
// MQTT client("https://gluebench.bubbleapps.io/", 1883, callback);

//SETUP
//  if(TheClient.connect(server, 443)) {
//         //Show when we Get the Connection to the Server
//         Serial.printf(".");
//         Serial.printf("Successfully Connected");
//     }
//     String subscriptionName = String::format("%s/%s/", System.deviceID().c_str(), EVENT_NAME);
//     Particle.subscribe(subscriptionName, subscriptionHandler, MY_DEVICES);
//     Serial.printf("Subscribing to %s\n", subscriptionName.c_str());

//LOOP
//  if((millis()-lastTime>10000)) {
//             // Send HTTP data
//             TheClient.printf("GET https:/gluebench.bubbleapps.io/version-test/api/1.1/wf/getMainTemp");
//             TheClient.printf("Host: gluebench.bubbleapps.io");
//             //received Data
//             receivedData = "";
//             // Read data from the buffer
//             if(TheClient.connect(server, 443)) { //443 for unsecured xfer, 80 is normal xfer
//             //Show when we Get the Connection to the Server
//                 Serial.printf(".");
//                 Serial.printf("Successfully Connected");
//                 while(receivedData.indexOf("\r\n\r\n") == -1) {
//                     memset(dataBuffer, 0x00, sizeof(dataBuffer));
//                     TheClient.read(dataBuffer, sizeof(dataBuffer));
//                     receivedData += (const char*)dataBuffer;
//                 }
//             lastTime = millis();
//             // Print the string
//                 Serial.printf("%s\n", receivedData);
//             // Stop the current connection
//                 // TheClient.stop();
//             }
//             else {
//                 Serial.printf("Server connection failed. Trying again...");
//             }
//         }