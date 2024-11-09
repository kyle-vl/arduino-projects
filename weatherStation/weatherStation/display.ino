// Define bit patterns for 0..9 and C
const uint8_t digit_patterns[] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F, // 9
  0x39  // C
};

// 4 digits to be displayed
uint8_t digits[4]= {0, 0, 0, digit_patterns[10]};
	
//The current digit of the 4-digit number we're displaying
uint8_t current_position = 0;

void display_init() {
  // Set anode pins to output
  DDRD = 0xFF;      // Note: Segment A -> D4, Segment B -> D5, etc. 
  DDRB |= 0x0F;     // Note: Segment E -> B0, Segment F -> B1, etc.

  // Set cathode pins to output
  DDRC |= 0x3C;     // Note: Position 1 -> C5, Position 2 -> C4, etc.
}

// Function that stores a given number in digit array
void load_digits(uint8_t number){
	for (int i = 2; i >= 0; i--) {
		digits[i] = digit_patterns[number % 10];       // Get rightmost digit
		number /= 10;                                  // Move to the next digit
	}
}

void display_digit() {
  uint8_t digit_pattern = digits[current_position];     // Get digit pattern for current digit
  PORTC |= 0x3C;                                        // Set all common cathodes to LOW
  PORTC &= ~(1 << (5 - current_position));              // Turn on current digit

  // Reset display
  PORTD &= 0x0F;                          // Turn off segments A..D
  PORTB &= 0xF0;                          // Turn off segments E..DP

  // Set digit pattern for current digit
  PORTD |= (digit_pattern & 0x0F) << 4;   // Set lower 4 bits to PORTD4..PORTD7
  PORTB |= (digit_pattern & 0xF0) >> 4;   // Set higher 4 bits to PORTB0..PORTB3  

  // Increment to next digit
  current_position++;
	if (current_position > 3) {
		current_position = 0;          // Reset after 3rd digit
	}
}