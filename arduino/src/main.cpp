
#include <Arduino.h>


extern void sk_setup();
extern void sk_loop();
extern void sk1_loop();

void setup() {

    Serial.begin(9600);
    Serial.print("Setup... ");
    //sk_setup();
    Serial.println("done.");
}


long prev_time = 0;
long draws = 0;

void loop() {

    if (millis() - prev_time > 1000)
    {
        Serial.print("looping...");
        Serial.println(draws);
        prev_time = millis();
        draws = 0;
    }
    draws += 1;
//    sk_loop();
    sk1_loop();
}
