#define F_CPU 16000000
#define BAUD 9600
#define DHT_PIN 0 // DHT pin connected to PIN C0

uint8_t data[5];                            // Array holds the 5 data bytes recorded by DHT11

void setup() {
  // Initialise UART and DHT11 and take measurement
  uart_init(BAUD);
  DHT11_init();
  DHT11_display();

  // Initialise 7 segment display and display timer
  timer0_init();
  display_init();

  // Enable interrupts
  sei();
}

void loop() {}

// This interrupt ensures all digits of the 7 segment display are on at the same time.
ISR(TIMER0_COMPA_vect) {
	display_digit();
}