#define F_CPU 16000000
#define BAUD 9600

void setup() {
  // put your setup code here, to run once:
  uart_init(BAUD);
}

void loop() {
  // put your main code here, to run repeatedly:
  uart_transmit_array("Testing\r\n");
}

void uart_init(uint16_t baud) {
  // Calculate ubrr value
	uint16_t ubrr = F_CPU / 16 / baud - 1;

  // Set baud rate
  cli();
  UBRR0H = (uint8_t)(ubrr >> 8); // Set baud rate high byte
	UBRR0L = (uint8_t)ubrr; // Set baud rate low byte
  sei();

  // Enable transmitter
  UCSR0B = (1 << TXEN0);
}

void uart_transmit_byte(uint8_t byte){
	while (!(UCSR0A & (1 << UDRE0))) {
		// Wait until the buffer is empty
	}
	UDR0 = byte; // Transmit data
}

void uart_transmit_array(char array[]){
	// Iterate over each character in message
	for (uint8_t i = 0; i < strlen(array); i++) {
		uart_transmit_byte(array[i]); // Transmit each character
	}
}