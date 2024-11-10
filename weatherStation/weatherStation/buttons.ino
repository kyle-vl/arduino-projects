void button_init() {
  DDRD &= ~(1 << BUTTON_PIN);
}