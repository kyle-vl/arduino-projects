#define F_CPU 16000000
#define BAUD 9600
#define DHT_PIN 0 // DHT pin connected to PIN C0
#define BUTTON_PIN 3

#include "DisplayState.h"

DisplayState currentDisplayState = DISPLAY_TEMPERATURE;  // Default to showing temperature
uint8_t data[5];                                         // Array holds the 5 data bytes recorded by DHT11
uint8_t humidity, temperature;
bool updateFlag = false;                                 // Flag to update whether display is showing temperature or humidity

void setup() {
  // Initialise UART and DHT11
  uart_init(BAUD);
  DHT11_init();

  // Take measurements
  DHT11_display(&temperature, &humidity);
  
  // Initialise 7 segment display, display timer, and button
  timer0_init();
  display_init();
  button_init();

  // Display measurements
  load_digits(temperature, currentDisplayState);

  // Enable interrupts
  sei();
}

void loop() {
  // When button is pressed, set the update flag
  while ((PIND & (1 << BUTTON_PIN)) != 0) {
    updateFlag = true;
  }

  // When update flag is set, toggle the display state
  if (updateFlag) {
    if (currentDisplayState == DISPLAY_TEMPERATURE) {
      // If current state is temperature
      currentDisplayState = DISPLAY_HUMIDITY;       // Change state to humidity
      load_digits(humidity, currentDisplayState);   // Load humidity value into display buffer
    } else {
      // If current state is humidity
      currentDisplayState = DISPLAY_TEMPERATURE;    // Change state to temperature
      load_digits(temperature, currentDisplayState);// Load temperature value into display buffer
    }
    
    updateFlag = false;     // Reset update flag
  }
}

// This interrupt ensures all digits of the 7 segment display are on at the same time.
ISR(TIMER0_COMPA_vect) {
	display_digit();
}