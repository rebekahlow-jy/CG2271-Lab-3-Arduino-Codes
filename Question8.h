#include <Arduino.h>
#include "prioq.h"
#define PIN_INT0  2
#define PIN_INT1  3

TPrioQueue *queue;

long lastDebounceTimeFirst = 0;  // the last time the output pin was toggled
long lastDebounceTimeSecond = 0;
long debounceDelay = 200;  // the debounce time; increase if the output flickers

void myDelay(int ms) {
	for (int i = 0; i < ms; i++) {
		delayMicroseconds(1000);
	}
}

// Declares a new type called "funcptr"

void (*funcptr)();

// Flashes LED at pin 7: 5 times a 4 Hz
void int0task() {
	digitalWrite(7, HIGH);
	myDelay(250);
	digitalWrite(7, LOW);
	myDelay(250);
}

// Flashes LED at pin 6: 5 times at 2HZ
void int1task() {
	digitalWrite(6, HIGH);
	myDelay(500);
	digitalWrite(6, LOW);
	myDelay(500);
}

void int0ISR() {
	/* switch de-bouncing */
	if ((millis() - lastDebounceTimeFirst) > debounceDelay) {
		lastDebounceTimeFirst = millis();
		for (int i = 0; i < 5; i++)
			enq(queue, (void *) int0task, 0);
	}

}

void int1ISR() {
	/* switch de-bouncing */
	if ((millis() - lastDebounceTimeSecond) > debounceDelay) {
		lastDebounceTimeSecond = millis();
		for (int i = 0; i < 5; i++)
			enq(queue, (void *) int1task, 1);
	}

}

void setup() {

	queue=makeQueue();
	attachInterrupt(digitalPinToInterrupt(PIN_INT0), int0ISR, RISING);
	attachInterrupt(digitalPinToInterrupt(PIN_INT1), int1ISR, RISING);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
}

// Dequeues and calls functions if the queue is not empty
void loop() {

	      // If we still have an item to dequeue
	      if(qlen(queue)>0)
	      {
	   	   funcptr = (void (*)()) (deq(queue));
	   	   funcptr();
	      }

}
