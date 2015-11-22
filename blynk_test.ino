//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_HardSer.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>
#include <SimpleTimer.h> // here is the SimpleTimer library

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

SimpleTimer timer; // Create a Timer object called "timer"!

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial && millis() < 8000){ // wait for serial port to connect. Needed for Leonardo only
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

  timer.setInterval(3000L, sendData);
}

float h=0;
float t=0;
int ti=0;

void sendData()
{
  Blynk.virtualWrite(V0, millis()/1000.0);
  Blynk.virtualWrite(V1, t);
  Serial.print("3sec\r\n");
}
void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  
  // Wait a few seconds between measurements.
  ti++;
  if( ti>= 200 )
  {
    ti = 0;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    t = dht.readTemperature();
    
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
    }
  }

  //Check connection status
  if ( !Blynk.connected() )
  {
    Serial.println("Retry connect");
    Blynk.connect();
  }
}
