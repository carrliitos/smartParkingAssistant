/** 
 * @author Benzon Carlitos Salazar
 * CS412 Final Arduino Project
*/

int redLED = 2;	// Red LED connected to pin 2
int yellowLED = 3; // Yellow LED connected to pin 3
int greenLED = 4; // Green LED connected to pin 4

int trigPin = 5; // Sensor Trig pin connected to pin 5
int echoPin = 6; // Sensor Echo pin connected to pin 6

int buzzer = 10; // Buzzer connected to pin 10

long TempDistance = 0; // Variable to store the temporary distance
int count = 0; // count variable to check if the object has stopped moving

void setup() {
	// Ultrasonic sensor
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	// LEDs
	pinMode(redLED, OUTPUT);
	pinMode(greenLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);

	// Buzzer
	pinMode(buzzer, OUTPUT);
	
	Serial.begin(9600);
}

void loop() {
	// Calculate distance in inches by reading values from sensor
	long duration, Distance;
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	Distance = (duration / 2) / 74; // distance in inches
	
	/**
	 * We start checking the values of each distance
	 * if Distance > 200, nothing in the garage -- turn off everything
	 * if 200 >= Distance > 55, Green LED turns on
	 * if 55 >= Distance > 15, Yellow LED turns on -- Caution
	 * if Distance <= 15, Red LED -- Warning
	 * if Distance < 8, Buzzer and Red LED -- Car way too close, DANGER!
	*/
	if(count < 10) {
		if(Distance > 200) {
			turnAllOff();
		}
		if((Distance > 55) && (Distance <= 200)) {
			digitalWrite(redLED, LOW);
			digitalWrite(yellowLED, LOW);
			digitalWrite(greenLED, HIGH);
			analogWrite(buzzer, 0);
		}
		if((Distance > 15) && (Distance <= 55)) {
			digitalWrite(redLED, LOW);
			digitalWrite(yellowLED, HIGH);
			digitalWrite(greenLED, LOW);
			analogWrite(buzzer, 0);
		}
		if(Distance <= 15) {
			digitalWrite(redLED, HIGH);
			digitalWrite(yellowLED, LOW);
			digitalWrite(greenLED, LOW);
			analogWrite(buzzer, 0);
		}
		if(Distance < 8) {
			analogWrite(buzzer, 500);
		}
	}
	
	/**
	 * Set the value of the count based on the car's movement
	 * which will decide when to turn off the LEDs.
	 * We compare the values of "Distance" to "TempDistance", and
	 * if the values are the same (no movement), we increment count.
	 * If the car moves, reset count to 0.
	 * Finally, set "TempDistance" to value of "Distance"
	*/
	if((Distance == TempDistance) || ((Distance + 1) == TempDistance) || ((Distance - 1) == TempDistance)) {
		if(count >= 10) { // turn off after 10 CPU cycles
			Serial.println("No movement detected.");
			turnAllOff();
		}else {
			count++;
			Serial.println(count);
		}
	}else {
		count = 0;
	}
	TempDistance = Distance;
	Serial.print(Distance);
	Serial.println(" inches");
	Serial.print("Count: ");
	Serial.println(count);
	delay(500);
}

// Function to turn off the LEDs and the buzzer
void turnAllOff() {
	digitalWrite(redLED, LOW);
	digitalWrite(greenLED, LOW);
	digitalWrite(yellowLED, LOW);
	analogWrite(buzzer, 0);
}