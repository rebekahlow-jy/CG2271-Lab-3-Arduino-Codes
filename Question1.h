#include <Arduino.h>

#define PIN_INT0  2
#define PIN_INT1  3

long lastDebounceTimeFirst = 0;  // the last time the output pin was toggled
long lastDebounceTimeSecond = 0;
long debounceDelay = 200;  // the debounce time; increase if the output flickers

void myDelay(int ms) {
	for (int i = 0; i < ms; i++) {
		delayMicroseconds(1000);
	}
}

void isr0() {

	/* switch de-bouncing */
	if ((millis() - lastDebounceTimeFirst) > debounceDelay) {
		lastDebounceTimeFirst = millis();
		/* Flash LED */
		for (int i = 0; i < 10; i++) {
			digitalWrite(6, HIGH);
			myDelay(1000);
			digitalWrite(6, LOW);
			myDelay(1000);
		}
	}
}

void isr1() {

	/* switch de-bouncing */
	if ((millis() - lastDebounceTimeSecond) > debounceDelay) {
		/* re-enable global interrupts */
		interrupts();
		lastDebounceTimeSecond = millis();
		/* Flash LED */
		for (int i = 0; i < 5; i++) {
			digitalWrite(7, HIGH);
			myDelay(1000);
			digitalWrite(7, LOW);
			myDelay(1000);
		}
	}

}

void setup() {
	attachInterrupt(digitalPinToInterrupt(PIN_INT0), isr0, RISING);
	attachInterrupt(digitalPinToInterrupt(PIN_INT1), isr1, RISING);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
}

void loop() {
}
