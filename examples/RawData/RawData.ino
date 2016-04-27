#include <Arduino.h>
#include <rn2483.h>


bool ledBLigth = false;
bool ledGLigth = false;
char buffer[100];

void setup() {
    SerialUSB.begin(57600);
    lora.init();
    delay(100);
    
    while (!SerialUSB) {
        ;
    }
    
    lora.rawData("sys get ver");
    lora.prepareAnswer(buffer, sizeof(buffer));
}

void loop() {
    char readData;     
    
    if (lora.hasAnswer()) {
        ledBlueLight(ledBLigth);
        ledBLigth = !ledBLigth;
        SerialUSB.write(lora.getLastAnswer());
    }
}

