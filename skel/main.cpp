#include <arduino.h>

#include <stdio.h>
#include <util/delay.h>

#define SERIAL_SPEED     115200

int main()
{
    init();

    Serial.begin(SERIAL_SPEED);
    Serial.println("start");

    for (;;)
    {
        Serial.println("alive");
        delay(500);
    }

    return 0;
}

