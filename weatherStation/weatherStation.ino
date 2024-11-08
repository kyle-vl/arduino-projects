#define F_CPU 16000000
#define BAUD 9600
#define DHT_PIN 2 // DHT pin connected to PIN D2

void setup() {
  // Initialise UART
  uart_init(BAUD);

  // Set DHT_PIN as output and high initally
  DDRD |= (1 << DHT_PIN);
  PORTD |= (1 << DHT_PIN);
}

void loop() {
   uint8_t data[5];               // Array holds the 5 data bytes recorded by DHT11
  if (DHT11_read(data)) {
    // Data is valid, print temperature and humidity

    // Create buffer for output string
    char buffer[50];

    uint8_t humidity = data[0];       // Humidity whole number byte == 0
    uint8_t temperature = data[2];    // Temperature whole number byte == 2

    // Format and send message
    snprintf(buffer, sizeof(buffer), "Humidity: %d%%, Temperature: %d°C\r\n", humidity, temperature);
    uart_transmit_array(buffer);  
  } else {
    // If the read failed, print a failure message
    uart_transmit_array("Failed to read data from DHT11 sensor\r\n");
  }
  delay(2000);                        // Wait before the next reading
}

void uart_init(uint16_t baud) {
  // Calculate ubrr value
	uint16_t ubrr = F_CPU / 16 / baud - 1;

  // Set baud rate
  cli();
  UBRR0H = (uint8_t)(ubrr >> 8);      // Set baud rate high byte
	UBRR0L = (uint8_t)ubrr;             // Set baud rate low byte
  sei();

  // Enable transmitter
  UCSR0B = (1 << TXEN0);
}

void uart_transmit_byte(uint8_t byte){
	while (!(UCSR0A & (1 << UDRE0))) {
		// Wait until the buffer is empty
	}
	UDR0 = byte;                        // Transmit data
}

void uart_transmit_array(char array[]){
	// Iterate over each character in message
	for (uint8_t i = 0; i < strlen(array); i++) {
		uart_transmit_byte(array[i]);     // Transmit each character
	}
}

// Function to read data from the DHT11 sensor
bool DHT11_read(uint8_t *data) {
  // Send a start signal to DHT11
  DDRD |= (1 << DHT_PIN);             // Set DHT_PIN to output to get data

  PORTD &= ~(1 << DHT_PIN);           // Set DHT_PIN to LOW
  delay(18);                          // Keep low for at least 18 ms
  PORTD |= (1 << DHT_PIN);            // Set DHT_PIN to HIGH
  delayMicroseconds(20);              // Wait 20 microseconds
  
  // Read response from DHT11
  pinMode(DHT_PIN, INPUT);            // Set DHT_PIN to input to read data
  delayMicroseconds(40);              // Wait for the DHT11 to respond
  
  /* Check for DHT11 response
  Explanation: The DHT11 response protocol pulls the line LOW for 80 microseconds,
  then HIGH for 80 microseconds - this signals it is ready to send the data.
  If this expected sequence doesn't occur, this tells us that no valid response was recieved.
  */
  if ((PIND & (1 << DHT_PIN)) == 0) {
    delayMicroseconds(80);
    if ((PIND & (1 << DHT_PIN)) != 0) {
      delayMicroseconds(80);
    } else {
      return false;
    }
  } else {
    return false;
  }

  // Read the 40 bits (5 bytes) of data
  for (int i = 0; i < 5; i++) {             // Loop over the 5 data bytes
    data[i] = 0;
    for (int j = 7; j >= 0; j--) {          // Loop over the 8 bits in each byte
      while ((PIND & (1 << DHT_PIN)) == 0); // Wait for the line to go HIGH
      delayMicroseconds(40);                // Wait for the bit to settle

      // Check if the bit is HIGH
      if ((PIND & (1 << DHT_PIN)) != 0) {   // If DHT_PIN is high, the bit is 1
        data[i] |= (1 << j);                // Set the corresponding bit to 1
      }
      while ((PIND & (1 << DHT_PIN)) != 0); // Wait for the line to go LOW
    }
  }

  return true;                              // Successfully read the data
}
