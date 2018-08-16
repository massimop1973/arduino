#include <CayenneMQTTESP8266.h>
#include <DHT.h>
#include <LedControl.h>

#define CAYENNE_PRINT Serial

#define button D1
#define led D2

char ssid[] = "NETGEAR";
char wifiPassword[] = "";
char username[] = "0119e290-98e0-11e8-ab4b-11d2d0c54106";
char password[] = "99a7436f4b2ea5865a1076f52ce647544cf55e14";
char clientID[] = "36795510-a15a-11e8-a59a-6d22310ed7cb";

unsigned long seconds;
int buttonPressed = 0;
long wifi_strength;
int UVsensorIn = A0; //Output from the sensor

void setup()
{
	pinMode(led, OUTPUT);
	pinMode(button, INPUT_PULLUP);
	pinMode(UVsensorIn, INPUT);
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
	attachInterrupt(digitalPinToInterrupt(button), interruptPulsante, FALLING);
}

void loop()
{
	seconds = millis();
	if (buttonPressed)
	{
		digitalWrite(led, HIGH);
	}
	else
	{
		digitalWrite(led, LOW);
	}
	wifi_strength = WiFi.RSSI();

	int uvLevel = averageAnalogRead(UVsensorIn);

	float outputVoltage = 3.3 * uvLevel / 1024;
	float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

	Serial.print(" UV Intensity: ");
	Serial.print(uvIntensity);
	Serial.print(" mW/cm^2");
	Serial.println();

	Cayenne.loop();
}

CAYENNE_OUT_DEFAULT()
{
	Cayenne.virtualWrite(0, seconds);
	Cayenne.luxWrite(1, wifi_strength);
	Cayenne.virtualWrite(2, buttonPressed);

	// Cayenne.celsiusWrite(2, hum);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	switch (request.channel)
	{
	case 3:
		buttonPressed = 0;
		break;
	}
}

void interruptPulsante()
{
	buttonPressed = 1;
}

int averageAnalogRead(int pinToRead)
{
	byte numberOfReadings = 8;
	unsigned int runningValue = 0;

	for (int x = 0; x < numberOfReadings; x++)
		runningValue += analogRead(pinToRead);
	runningValue /= numberOfReadings;

	return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}