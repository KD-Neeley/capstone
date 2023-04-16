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

//OLED
const int OLED_RESET = D4;

//Distance Sensor
const int MOTIONSENSOR = A1;
int distance;
int distanceThreshold = 2900;


//Objects
Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, WS2812B);
Adafruit_SSD1306 myDisplay(OLED_RESET); 
//Objects for ATTEMPT_v1.0
TCPClient client;                           // Create TCP Client object
byte server[] = { 104, 19, 217, 48 }; 		//https://gluebench.bubbleapps.io/
byte dataBuffer[1024];
String receivedData;


//function prototype
int pixelFill (int startPixel, int endPixel, int brightness, int hexColor);
int tradRainbow (int startPixel, int endPixel, int hexColor);
int movingTradRainbow (int startPixel, int endPixel, int hexColor);


SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 15000);
    pixel.begin();
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
    delay(1);
    digitalWrite(RESETPIN, LOW);
    digitalWrite(STROBE, HIGH); 
    delay(1);

//Initialize the OLED
    myDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    myDisplay.clearDisplay();

//Initialize the IR Distance Sensor
    pinMode(MOTIONSENSOR, INPUT);
    

}

void loop() {
    distance = analogRead(MOTIONSENSOR); //Get Distance to determine if the program will run given a presence
    Serial.printf("Distance = %i\n", distance); //Troubleshoot: Print Distance

    //GET Sound
    // Cycle through each frequency band by pulsing the strobe.
    for(int i = 0; i < 7; i++) {
        digitalWrite(STROBE, LOW);
        delay(1);
        soundLevel[i] = analogRead(OUT);
        digitalWrite(STROBE, HIGH);
        delay(1); 
    }
    for(int i=0; i<7; i++) {
        Serial.printf("Level %i\n Value %i\n", i, soundLevel[i]);
    }
    if(distance > distanceThreshold) { //if a presence is sensed run the program {
        //OLED Display properties
        myDisplay.setRotation(2);
        myDisplay.setCursor (8,1);
        myDisplay.setTextSize(2);
        myDisplay.setTextColor(WHITE);
        myDisplay.printf("Distance = %i\n", distance); //Troublehsoot: Print Distance
        myDisplay.display();
        myDisplay.clearDisplay();
        myDisplay.display();

        //Light Iteration Variables for Neopixel settings
        static int upDown = 1;
        static int pixelNum = 0;
        static int lastPixelTime = 0;
        int maxPixel = 16;
        int minPixel = 0;

        if(millis()-lastPixelTime > 500) {  //start timer
            lastPixelTime = millis();       //reset timer


            if(soundLevel[0] <= 63){
                pixelFill(0, 4, 50, fullred);
            }
            if(soundLevel[0] <= 160 && soundLevel[0] > 63){
                pixelFill(0, 4, 85, fullred);
            }
            if(soundLevel[0] <= 400 && soundLevel[0] > 160){
                pixelFill(0, 4, 100, fullred);
            }
            if(soundLevel[0] <= 1000 && soundLevel[0] > 400){
                pixelFill(0, 4, 150, fullred);
            }
            if(soundLevel[0] <= 2500 && soundLevel[0] > 1000){
                pixelFill(0, 4, 200, fullred);
            }
            if(soundLevel[0] <= 6250 && soundLevel[0] > 2500){
                pixelFill(0, 4, 220, fullred);
            }
            if(soundLevel[0] <= 16000 && soundLevel[0] > 6250){
                pixelFill(0, 4, 255, fullred);
            }
        
            //Light effects: Sound Level 1
            if(soundLevel[1] <= 63){
                pixelFill(5, 6, 50, fullmagenta);
            }
            if(soundLevel[1] <= 160 && soundLevel[0] > 63){
                pixelFill(5, 6, 85, fullmagenta);
            }
            if(soundLevel[1] <= 400 && soundLevel[0] > 160){
                pixelFill(5, 6, 100, fullmagenta);
            }
            if(soundLevel[1] <= 1000 && soundLevel[0] > 400){
                pixelFill(5, 6, 150, fullmagenta);
            }
            if(soundLevel[1] <= 2500 && soundLevel[0] > 1000){
                pixelFill(5, 6, 200, fullmagenta);
            }
            if(soundLevel[1] <= 6250 && soundLevel[0] > 2500){
                pixelFill(5, 6, 220, fullmagenta);
            }
            if(soundLevel[1] <= 16000 && soundLevel[0] > 6250){
                pixelFill(5, 6, 255, fullmagenta);
            }

            //Light effects: Sound Level 2
            if(soundLevel[2] <= 63){
                pixelFill(7, 8, 50, purple);
            }
            if(soundLevel[2] <= 160 && soundLevel[0] > 63){
                pixelFill(7, 8, 85, purple);
            }
            if(soundLevel[2] <= 400 && soundLevel[0] > 160){
                pixelFill(7, 8, 100, purple);
            }
            if(soundLevel[2] <= 1000 && soundLevel[0] > 400){
                pixelFill(7, 8, 150, purple);
            }
            if(soundLevel[2] <= 2500 && soundLevel[0] > 1000){
                pixelFill(7, 8, 200, purple);
            }
            if(soundLevel[2] <= 6250 && soundLevel[0] > 2500){
                pixelFill(7, 8, 220, purple);
            }
            if(soundLevel[2] <= 16000 && soundLevel[0] > 6250){
                pixelFill(7, 8, 255, purple);
            }


            //Light effects: Sound Level 3
            if(soundLevel[3] <= 63){
                pixelFill(9, 12, 50, fullblue);
            }
            if(soundLevel[3] <= 160 && soundLevel[0] > 63){
                pixelFill(9, 12, 85, fullblue);
            }
            if(soundLevel[3] <= 400 && soundLevel[0] > 160){
                pixelFill(9, 12, 100, fullblue);
            }
            if(soundLevel[3] <= 1000 && soundLevel[0] > 400){
                pixelFill(9, 12, 150, fullblue);
            }
            if(soundLevel[3] <= 2500 && soundLevel[0] > 1000){
                pixelFill(9, 12, 200, fullblue);
            }
            if(soundLevel[3] <= 6250 && soundLevel[0] > 2500){
                pixelFill(9, 12, 220, fullblue);
            }
            if(soundLevel[3] <= 16000 && soundLevel[0] > 6250){
                pixelFill(9, 12, 255, fullblue);
            }


            //Light effects: Sound Level 4
            if(soundLevel[4] <= 63){
                pixelFill(13, 14, 50, fullgreen);
            }
            if(soundLevel[4] <= 160 && soundLevel[0] > 63){
                pixelFill(13, 14, 85, fullgreen);
            }
            if(soundLevel[4] <= 400 && soundLevel[0] > 160){
                pixelFill(13, 14, 100, fullgreen);
            }
            if(soundLevel[4] <= 1000 && soundLevel[0] > 400){
                pixelFill(13, 14, 150, fullgreen);
            }
            if(soundLevel[4] <= 2500 && soundLevel[0] > 1000){
                pixelFill(13, 14, 200, fullgreen);
            }
            if(soundLevel[4] <= 6250 && soundLevel[0] > 2500){
                pixelFill(13, 14, 220, fullgreen);
            }
            if(soundLevel[4] <= 16000 && soundLevel[0] > 6250){
                pixelFill(13, 14, 255, fullgreen);
            }


            //Light effects: Sound Level 5
            if(soundLevel[5] <= 63){
                pixelFill(15, 15, 50, fullyellow);
            }
            if(soundLevel[5] <= 160 && soundLevel[0] > 63){
                pixelFill(15, 15, 85, fullyellow);
            }
            if(soundLevel[5] <= 400 && soundLevel[0] > 160){
                pixelFill(15, 15, 100, fullyellow);
            }
            if(soundLevel[5] <= 1000 && soundLevel[0] > 400){
                pixelFill(15, 15, 150, fullyellow);
            }
            if(soundLevel[5] <= 2500 && soundLevel[0] > 1000){
                pixelFill(15, 15, 200, fullyellow);
            }
            if(soundLevel[5] <= 6250 && soundLevel[0] > 2500){
                pixelFill(15, 15, 220, fullyellow);
            }
            if(soundLevel[5] <= 16000 && soundLevel[0] > 6250){
                pixelFill(15, 15, 255, fullyellow);
            }

           //Light effects: Sound Level 6
            if(soundLevel[6] <= 63){
                pixelFill(16, 16, 50, orange);
            }
            if(soundLevel[6] <= 160 && soundLevel[0] > 63){
                pixelFill(16, 16, 85, orange);
            }
            if(soundLevel[6] <= 400 && soundLevel[0] > 160){
                pixelFill(16, 16, 100, orange);
            }
            if(soundLevel[6] <= 1000 && soundLevel[0] > 400){
                pixelFill(16, 16, 150, orange);
            }
            if(soundLevel[6] <= 2500 && soundLevel[0] > 1000){
                pixelFill(16, 16, 200, orange);
            }
            if(soundLevel[6] <= 6250 && soundLevel[0] > 2500){
                pixelFill(16, 16, 220, orange);
            }
            if(soundLevel[6] <= 16000 && soundLevel[0] > 6250){
                pixelFill(16, 16, 255, orange);
            }
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
                            
        //                     Serial.printf("sound levels");  
        //             }
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