/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/kdneeley/Documents/IoT/capstone/prototype_solarPoweredParkBench/src/prototype_solarPoweredParkBench.ino"
/*
 * Project prototype_solarPoweredParkBench
 * Description: The Solar Powered Park Bench will provide visitors with the ability to
 *              charge their devices using a USB port. The lights on the bench will
 *              provide mild entertainment by responding to sounds in the environment.
 * 
 *              Get sound detection using the MSGEQ7 sound detector 
 *              Make NeoPixels respond to the sound input
 *              Detect the presence of someone sitting on the bench 
 *              Put the program in sleep mode when no presence is detected
 *              Wake the program and run it when a presence is detected
 *              Power the microcontroller using a Solar Panel 
 *              Charge a 12v battery using the Solar Panel
 *              Switch power to battery operation when the sun is gone
 *              Provide a charge to a USB port to charge common devices (Phones, Ipads, etc)
 *              Get the train and bus schedules for the installation site
 *              Display the train and bus schedules on the OLED
 *              Display the current time and date on the OLED
 *              Display the current temperature on the OLED
 *              Publish the data to a dashboard
 * 
 * Author: Katie Neeley
 * Date: 04/11/2023
 */
#include <neopixel.h>
#include "kdsRainbows.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

//GLOBALS

//Neopixels
void setup();
void loop();
#line 33 "/Users/kdneeley/Documents/IoT/capstone/prototype_solarPoweredParkBench/src/prototype_solarPoweredParkBench.ino"
int const PIXELCOUNT= 16;
int const PIXELPIN = D8;
int startPixel;
int endPixel;
int hexColor;
int brightness;

//OLED
const int OLED_RESET = D4;

//Sound Detector

//Distance Sensor
const int MOTIONSNSR = A1;
int distance;


//Objects
Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, WS2812B);
Adafruit_SSD1306 myDisplay(OLED_RESET); 
//Objects for ATTEMPT_v1.0
TCPClient client;                           // Create TCP Client object
byte server[] = { 104, 19, 217, 48 }; 		//https://gluebench.bubbleapps.io/
byte dataBuffer[1024];
String receivedData;


//function prototype
int tradRainbow (int startPixel, int endPixel, int hexColor);
int movingTradRainbow (int startPixel, int endPixel, int hexColor);


SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 15000);
    // WiFi.on();
    // WiFi.connect();
    // while(WiFi.connecting()) {
    //     Serial.printf(".");
    // }
    // Serial.printf("\n\n");
    pixel.begin();

    myDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    myDisplay.clearDisplay();
    pinMode(MOTIONSNSR, INPUT);
}

void loop() {

    while(tradRainbow(0, PIXELCOUNT, fullgreen)) {
        distance = analogRead(MOTIONSNSR);
        Serial.printf("Distance = %i\n", distance);
        myDisplay.setRotation(2);
        myDisplay.setCursor (8,1);
        myDisplay.setTextSize(2);
        myDisplay.setTextColor(WHITE);
        myDisplay.printf("Distance = %i\n", distance);
        myDisplay.display();
        delay(3000);
        myDisplay.clearDisplay();
        myDisplay.display();
    }

    //ATTEMPT V1.0
    // if(client.connect(server, 80))
    // {
    //     //Show when we Get the Connection to the Server
    //     Serial.printf(".");
    //     Serial.printf("Successfully Connected");
                
    //     // Send HTTP data
    //     client.printf("GET https://gluebench.bubbleapps.io//version-test/api/OpenWeather-UsingLat&Long|Data/wf/sendWeatherReport");
    //     client.printf("Host: gluebench.bubbleapps.io");
    //     //received Data
    //     receivedData = "";
    //     // Read data from the buffer
    //     while(receivedData.indexOf("\r\n\r\n") == -1)
    //     {
    //         memset(dataBuffer, 0x00, sizeof(dataBuffer));
    //         client.read(dataBuffer, sizeof(dataBuffer));
    //         receivedData += (const char*)dataBuffer;
    //     }
    //     // Print the string
    //    Serial.printf("%c\n", receivedData);
    //     // Stop the current connection
    //     client.stop();
    // }  
    // else
    // {
    //     Serial.println("Server connection failed. Trying again...");
    // }

}

////FUNCTION DEFINITIONS
//NEOPIXEL FUNCTIONS
int tradRainbow (int startPixel, int endPixel, int hexColor) {
    int maxBrightness = 255;
    int minBrightness = 20;
    int startColor = 0;
    int endColor = 9;

    for(int b=minBrightness; b<=maxBrightness; b++) {
        brightness = b;
        pixel.setBrightness(b);
        for(int c = startColor; c <= endColor; c++) {
            hexColor = traditionalrainbow[c];
            for(int i=startPixel; i<endPixel+1; i++) {
                    pixel.setPixelColor(i, hexColor);
                    pixel.show(); 
                }
            
            delay(100);
        }
    }
    for(int b = maxBrightness; b >= minBrightness; b--) {
        pixel.setBrightness(b);
        for(int c = endColor; c >= startColor; c--) {
            hexColor = traditionalrainbow[c];
            for(int i=startPixel; i<endPixel+1; i++) {
                    pixel.setPixelColor(i, hexColor);
                    pixel.show(); 
                }
            delay(100);
        }
    }
    pixel.show();

    return(endPixel);
}

int movingTradRainbow (int startPixel, int endPixel, int hexColor) {
    int maxBrightness = 255;
    int minBrightness = 20;
    int startColor = 0;
    int endColor = 9;

    for(int b=minBrightness; b<= maxBrightness; b++) {
        pixel.setBrightness(b);
        Serial.printf("Brightness = %i \n", b);
        for(int i=startPixel; i<endPixel+1; i++) {
            for(int c = startColor; c <= endColor; c++) {
                hexColor = traditionalrainbow[c];
                        pixel.setPixelColor(i, hexColor);
                        pixel.show(); 
                        delay(15);
                        pixel.clear();
                        pixel.show();
                }
          
        }

    }
    for(int b = maxBrightness; b >= minBrightness; b--) {
        pixel.setBrightness(b);
        for(int i = endPixel; i>= startPixel; i--) {
            for(int c = endColor; c >= startColor; c--) {
                hexColor = traditionalrainbow[c];
                        pixel.setPixelColor(i, hexColor);
                        pixel.show(); 
                        delay(15);
                        pixel.clear();
                        pixel.show();
            }
            
        }
    }
    pixel.show();
    return (endPixel);
}