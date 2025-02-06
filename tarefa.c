#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define OLED_RESET    -1
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

#define WS2812_PIN  7
#define NUM_LEDS    25
#define BUTTON_A    5
#define BUTTON_B    6
#define LED_RED     11
#define LED_GREEN   12
#define LED_BLUE    13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_NeoPixel matrix(NUM_LEDS, WS2812_PIN, NEO_GRB + NEO_KHZ800);

volatile bool buttonA_pressed = false;
volatile bool buttonB_pressed = false;
bool ledGreenState = false;
bool ledBlueState = false;

void IRAM_ATTR handleButtonA() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 200) {
        buttonA_pressed = true;
    }
    last_interrupt_time = interrupt_time;
}

void IRAM_ATTR handleButtonB() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 200) {
        buttonB_pressed = true;
    }
    last_interrupt_time = interrupt_time;
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(BUTTON_A), handleButtonA, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_B), handleButtonB, FALLING);
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 error!");
        while (1);
    }
    display.clearDisplay();
    display.display();
    
    matrix.begin();
    matrix.show();
}

void loop() {
    if (Serial.available() > 0) {
        char receivedChar = Serial.read();
        Serial.print("Recebido: ");
        Serial.println(receivedChar);
        
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 20);
        display.print(receivedChar);
        display.display();

        if (receivedChar >= '0' && receivedChar <= '9') {
            int num = receivedChar - '0';
            showSymbol(num);
        }
    }

    if (buttonA_pressed) {
        ledGreenState = !ledGreenState;
        digitalWrite(LED_GREEN, ledGreenState);
        
        Serial.println(ledGreenState ? "LED Verde ON" : "LED Verde OFF");
        displayMessage(ledGreenState ? "LED Verde ON" : "LED Verde OFF");
        buttonA_pressed = false;
    }
    
    if (buttonB_pressed) {
        ledBlueState = !ledBlueState;
        digitalWrite(LED_BLUE, ledBlueState);
        
        Serial.println(ledBlueState ? "LED Azul ON" : "LED Azul OFF");
        displayMessage(ledBlueState ? "LED Azul ON" : "LED Azul OFF");
        buttonB_pressed = false;
    }
}

void displayMessage(const char* message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 30);
    display.print(message);
    display.display();
}

void showSymbol(int num) {
    matrix.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
        matrix.setPixelColor(i, (num % 2 == 0) ? matrix.Color(0, 255, 0) : matrix.Color(255, 0, 0));
    }
    matrix.show();
}
