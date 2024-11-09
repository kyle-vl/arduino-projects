#define F_CPU 16000000
#define BAUD 9600
#define DHT_PIN 2 // DHT pin connected to PIN D2

void setup() {
  // Initialise UART and DHT11
  uart_init(BAUD);
  DHT11_init();
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
