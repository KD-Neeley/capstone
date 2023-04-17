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
#include <MQTT.h>
#include <JsonParserGeneratorRK.h>
#include <HttpClient.h>

//GLOBALS
//MSGEQ7 Sound Sensor
void setup();
void loop();
int pixelFill(int startPixel, int endPixel, int brightness, int hexColor);
#line 35 "/Users/kdneeley/Documents/IoT/capstone/prototype_solarPoweredParkBench/src/prototype_solarPoweredParkBench.ino"
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

//OLED
const int OLED_RESET = D4;

//Distance Sensor
const int MOTIONSENSOR = A1;
int distance;
int distanceThreshold = 2817;

//JSON Xfer
int const PORT=80;
int nextTime;
int lastTime;
int mainTemp;

//function prototype
int pixelFill (int startPixel, int endPixel, int brightness, int hexColor);
int tradRainbow (int startPixel, int endPixel, int hexColor);
int movingTradRainbow (int startPixel, int endPixel, int hexColor);

//Objects
Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, WS2812B);
Adafruit_SSD1306 myDisplay(OLED_RESET); 
//Objects for JSON Xfer
TCPClient TheClient; // Create TCP Client object  
HttpClient http;
http_request_t request;
http_response_t response;
JsonParserStatic<512, 16> jsonParser;                       
byte server[] = { 104, 19, 217, 48}; //https://gluebench.bubbleapps.io/



SYSTEM_MODE(AUTOMATIC);

void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 15000);
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
    Serial.printf("first read is %i", distance);

    if(distance > distanceThreshold) { //if a presence is sensed run the program
        //OLED Display properties
        // myDisplay.clearDisplay();
        // myDisplay.display();
        // myDisplay.setRotation(2);
        // myDisplay.setCursor (1,1);
        // myDisplay.setTextSize(2);
        // myDisplay.setTextColor(WHITE);
        // myDisplay.printf("Distance = %i\n", distance); //Troublehsoot: Print Distance
        // myDisplay.display();
     

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
        Serial.printf("Level 0\n Value %i\n", soundLevel[1]);
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
        Serial.printf("Level 0\n Value %i\n", soundLevel[2]);
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
        Serial.printf("Level 0\n Value %i\n", soundLevel[3]);
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
        Serial.printf("Level 0\n Value %i\n", soundLevel[4]);
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
        Serial.printf("Level 0\n Value %i\n", soundLevel[5]);
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
        Serial.printf("Level 0\n Value %i\n", soundLevel[6]);
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

        //Get the Temperature data from Bubble
       
       if(WiFi.connecting() == false) {
          if((millis()-lastTime > 60000)) {
            request.hostname = "gluebench.bubbleapps.io";
            request.port = PORT;
            request.path = "/version-test/api/1.1/wf/getMainTemp";
            http.get(request, response);
            if (response.status == 200) {
                jsonParser.clear();
                Serial.print("Application>\tResponse status: ");
                Serial.println(response.status);
                Serial.print("Application>\tHTTP Response Body: ");
                Serial.println(response.body);
                mainTemp = jsonParser.parse();
            
            } 
            lastTime = millis();
          }
            else {
                Serial.printf("HTTP error: ");
                Serial.println(response.status);
            }
            delay(5000);
       }
    }
    else {
        distance = analogRead(MOTIONSENSOR);
    }

    
}

////FUNCTION DEFINITIONS
//NEOPIXEL FUNCTIONS
int pixelFill(int startPixel, int endPixel, int brightness, int hexColor) {
            pixel.clear();
            pixel.show();
            pixel.setBrightness(brightness);
            for(int i = startPixel; i < endPixel; i++) {
                pixel.setPixelColor(i, hexColor);
            }
            pixel.show();
            delay(100);
            pixel.clear();
            pixel.show();

            return(endPixel);
}

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