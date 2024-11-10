void DHT11_init() {
  // Set DHT_PIN as output and high initally
  DDRC |= (1 << DHT_PIN);
  PORTC |= (1 << DHT_PIN);
}

// Function to read data from the DHT11 sensor
bool DHT11_read(uint8_t *data) {
  // Send a start signal to DHT11
  DDRC |= (1 << DHT_PIN);             // Set DHT_PIN to output to get data

  PORTC &= ~(1 << DHT_PIN);           // Set DHT_PIN to LOW
  delay(18);                          // Keep low for at least 18 ms
  PORTC |= (1 << DHT_PIN);            // Set DHT_PIN to HIGH
  delayMicroseconds(20);              // Wait 20 microseconds
  
  // Read response from DHT11
  DDRC &= ~(1 << DHT_PIN);            // Set DHT_PIN to input to read data
  delayMicroseconds(40);              // Wait for the DHT11 to respond
  
  // Check for DHT11 response
  if ((PINC & (1 << DHT_PIN)) == 0) {
    delayMicroseconds(80);                  // DHT11 pulls line LOW for 80 microseconds
    if ((PINC & (1 << DHT_PIN)) != 0) {
      delayMicroseconds(80);                // DHT11 pulls line HIGH for 80 microseconds
    } else {
      return false;                         // Response error
    }
  } else {
    return false;                           // Response error
  }

  // Read the 40 bits (5 bytes) of data
  for (int i = 0; i < 5; i++) {             // Loop over the 5 data bytes
    data[i] = 0;
    for (int j = 7; j >= 0; j--) {          // Loop over the 8 bits in each byte
      while ((PINC & (1 << DHT_PIN)) == 0); // Wait for the line to go HIGH
      delayMicroseconds(40);                // Wait for the bit to settle

      // Check if the bit is HIGH
      if ((PINC & (1 << DHT_PIN)) != 0) {   // If DHT_PIN is high, the bit is 1
        data[i] |= (1 << j);                // Set the corresponding bit to 1
      }
      while ((PINC & (1 << DHT_PIN)) != 0); // Wait for the line to go LOW
    }
  }

  // Verify checksum
  uint8_t checksum = data[0] + data[1] + data[2] + data[3];
  if (checksum != data[4]) {
    return false;                           // Data transmission error
  }

  return true;                  // Successfully verify data
}

// Function to display data from the DHT11 sensor
void DHT11_display(uint8_t *temperature, uint8_t *humidity) {
  if (DHT11_read(data)) {
    // Data is valid, store temperature and humidity values
    *humidity = data[0];       // Humidity whole number byte == 0
    *temperature = data[2];    // Temperature whole number byte == 2

    // Create buffer for output string
    char buffer[50];

    // Format and send message
    snprintf(buffer, sizeof(buffer), "Humidity: %d%%, Temperature: %d°C\r\n", *humidity, *temperature);
    uart_transmit_array(buffer);
  } else {
    // If the read failed, print a failure message
    uart_transmit_array("Failed to read data from DHT11 sensor\r\n");
  }
}