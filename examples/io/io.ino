#include <io.h>

void setup() {
    Serial.begin(115200);
    ioInit();
}

void loop() {
    ioLoop();
}
