// This timer records the refresh rate of the 7 segment display
void timer0_init(){
	TCCR0A = (1 << WGM01);                // Set to timer to CTC (Clear Timer on Compare Match) mode
	TCCR0B = (1 << CS02) | (1 << CS00);   // Set prescaler to 1024
	TIMSK0 = (1 << OCIE0A);               // Enable Timer0 Compare Match A interrupt
	OCR0A = 60;                           // Set CTC every 400 microseconds
}