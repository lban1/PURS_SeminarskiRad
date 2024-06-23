#include <AM2302-Sensor.h>
#include <SoftwareSerial.h>

AM2302::AM2302_Sensor am2302{2};
SoftwareSerial comSerial(10, 11); // RX, TX

void setup()
{
  Serial.begin(9600);
  comSerial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);

  am2302.begin();
}

void loop()
{
  
  auto status = am2302.read();

  if (status == AM2302::AM2302_READ_OK) {

    //digitalWrite(LED_BUILTIN, HIGH);

    //Sending data
    comSerial.print("Temperature: ");
    comSerial.print(am2302.get_Temperature());
    comSerial.print(", Humidity: ");
    comSerial.print(am2302.get_Humidity());
    comSerial.println("%");

    //Read data from sensor and print on terminal
    Serial.print("Temperature: ");
    Serial.println(am2302.get_Temperature());
    Serial.print("Humidity: ");
    Serial.println(am2302.get_Humidity());
    
    //delay(500);
    //digitalWrite(LED_BUILTIN, LOW);

  } //else {
    //Serial.print("\n\nStatus of sensor read(): ");
    //Serial.println(status);
  //}

  //delay(1000);
}