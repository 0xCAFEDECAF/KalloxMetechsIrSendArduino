
#include <IRremote.h>

// For Arduino Uno: pin 3
IRsend irsend;

// Mark and space, in microseconds. Looks like it must add up to 1690.
#define B_0 430,1260
#define B_1 1300,390
#define B_0_LARGE_GAP 430,34755
#define B_0_SMALL_GAP 430,22874

const unsigned int CodesClose[] PROGMEM =
{
    B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_0, B_1, B_0, B_0, B_0_LARGE_GAP,
    B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_0, B_1, B_0, B_0, B_0_SMALL_GAP,
    B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_0, B_1, B_0, B_0, B_0_LARGE_GAP,

    B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0_LARGE_GAP,
};

const unsigned int CodesOpen[] PROGMEM =
{
    B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_0_LARGE_GAP,
    B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_0_SMALL_GAP,
    B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_1, B_0, B_0, B_0, B_0_LARGE_GAP,

    B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0_LARGE_GAP,
};

const unsigned int CodesStop[] PROGMEM =
{
    B_0, B_0, B_0, B_1, B_0, B_0, B_1, B_0, B_0, B_0, B_0, B_0_LARGE_GAP,
    B_0, B_0, B_0, B_1, B_0, B_0, B_1, B_0, B_0, B_0, B_0, B_0_SMALL_GAP,
    B_0, B_0, B_0, B_1, B_0, B_0, B_1, B_0, B_0, B_0, B_0, B_0_LARGE_GAP,

    B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0, B_0_LARGE_GAP,
};

char recvOneChar()
{
    if (Serial.available() > 0) return Serial.read();
    return 0;
} // recvOneChar

#define KHZ 38

void sendIrCodes(unsigned int* pgm_array, unsigned int* buffer, int n)
{
    for (int i = 0; i < n; i++) buffer[i] = pgm_read_word(&pgm_array[i]);

    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on
    irsend.sendRaw(buffer, n, KHZ);
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off
} // sendIrCodes

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);  // Initialize onboard LED as output 
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off

    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
    Serial.println();
    Serial.println("To close curtain, enter 'c' and press the 'Send' button. Enter 'o' to open, and 's' to stop.");
} // setup

void loop()
{
    char c = recvOneChar();
    if (c == 'c')
    {
        const int n = sizeof(CodesClose) / sizeof(CodesClose[0]);
        unsigned int buffer[n];

        Serial.println("Closing...");
        sendIrCodes(CodesClose, buffer, n);
    }
    else if (c == 'o')
    {
        const int n = sizeof(CodesOpen) / sizeof(CodesOpen[0]);
        unsigned int buffer[n];

        Serial.println("Opening...");
        sendIrCodes(CodesOpen, buffer, n);
    } // if
    else if (c == 's')
    {
        const int n = sizeof(CodesStop) / sizeof(CodesStop[0]);
        unsigned int buffer[n];

        Serial.println("Stopping...");
        sendIrCodes(CodesStop, buffer, n);
    } // if
} // loop
