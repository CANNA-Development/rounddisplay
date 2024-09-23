
#ifndef ASIM
    #include <Arduino.h>
#endif

extern void sketch_setup();
extern void sketch_loop();
extern void sketch_loop1();

void setup() {

    Serial.begin(9600);
    Serial.print("Setup... ");
    //sketch_setup();
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
    sketch_loop1();
}
