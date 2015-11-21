//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_HardSer.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>

// Set ESP8266 Serial object
#define EspSerial Serial1

ESP8266 wifi(EspSerial);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c6ec3d8dac57479591deddfec6a70633";

#include <DHT.h>

int RXLED = 17;

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2     // what digital pin we're connected to

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial){ // wait for serial port to connect. Needed for Leonardo only
    digitalWrite(RXLED, LOW);   // set the LED on
    delay(100);
    digitalWrite(RXLED, HIGH);   // set the LED off
    delay(100);
  }
   
  Serial.print("setup begin\r\n");
  EspSerial.begin(115200);
  delay(10);

  Blynk.begin(auth, wifi, "plop", "plopplop");
  dht.begin();
}

int timer = 0;
void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  
  // Wait a few seconds between measurements.
  timer++;
  if( timer>= 200 )
  {
    timer = 0;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) ) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else
    {
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C ");
      Blynk.virtualWrite(V1, t);
    }
  }
}
