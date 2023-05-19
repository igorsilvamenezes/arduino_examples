// Pin number for the buzzer
#define BUZZER_PIN 6



void beep(unsigned int note) {
  tone(BUZZER_PIN, note, 100);
  delay(150);
  noTone(BUZZER_PIN);
}
