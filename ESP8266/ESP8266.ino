#include <CayenneMQTTESP8266.h>
#include <DHT.h>
#include <LedControl.h>
#define CAYENNE_PRINT Serial
#define dht_pin D2
#define dht_type DHT11

char ssid[] = "NETGEAR";
char wifiPassword[] = "";
char username[] = "0119e290-98e0-11e8-ab4b-11d2d0c54106";
char password[] = "99a7436f4b2ea5865a1076f52ce647544cf55e14";
char clientID[] = "50094dd0-9e41-11e8-9300-637b1fa64cc6";

unsigned long seconds;
float temp;
float hum;
int din = 16; // D0

int CS =  5;  // D1

int CLK = 4;  // D2

DHT dht(dht_pin, dht_type);

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
	dht.begin();
}

void loop() {
	seconds=millis();
	hum = dht.readHumidity();
    temp= dht.readTemperature();
	Cayenne.loop();
}

CAYENNE_OUT_DEFAULT()
{
	Cayenne.virtualWrite(0, seconds);
	Cayenne.celsiusWrite(1, temp);
	Cayenne.celsiusWrite(2, hum);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	switch (request.channel) {
		case 3: // button
			Serial.print("button");
			Serial.println(getValue.asString());
			break;
		case 4: // fade
			Serial.print("fade");
			Serial.println(getValue.asString());
	}
}