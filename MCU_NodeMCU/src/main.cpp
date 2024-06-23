/*#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial comSerial(D5, D6); // RX, TX
String recievedString;

float parseTemperature(const String& data);
float parseHumidity(const String& data);

void setup()
{
  Serial.begin(9600);
  comSerial.begin(9600);
}

void loop()
{
  // Read data from sender
  if (comSerial.available())
  {
    recievedString = comSerial.readStringUntil('\n');
    Serial.println(recievedString);

    float temperature = parseTemperature(recievedString);
    float humidity = parseHumidity(recievedString);

    Serial.print("Parsed Temperature: ");
    Serial.println(temperature);
    Serial.print("Parsed Humidity: ");
    Serial.println(humidity);
    delay(1000);
  }else{
    Serial.println("Nije procitano");
    delay(1000);
  }
}

float parseTemperature(const String& data) {
  int tempIndex = data.indexOf("Temperature: ");
  if (tempIndex != -1) {
    int start = tempIndex + strlen("Temperature: ");
    int end = data.indexOf(" ", start);
    if (end != -1) {
     return data.substring(start, end).toFloat();
    }
  }
  return NAN; // Return NaN if not found
}

float parseHumidity(const String& data) {
  int humIndex = data.indexOf(", Humidity: ");
  if (humIndex != -1) {
    int start = humIndex + strlen(", Humidity: ");
    int end = data.indexOf("%", start);
     if (end != -1) {
     return data.substring(start, end).toFloat();
    }
  }
  return NAN; // Return NaN if not found
}*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "blondie";
const char* password = "blondie76";
const char* serverName = "http://192.168.2.129/temperatura"; // URL prema Flask aplikaciji

SoftwareSerial comSerial(D5, D6); // RX, TX
String recievedString;

float temperature = 0.0;
float humidity = 0.0;

float parseTemperature(const String& data);
float parseHumidity(const String& data);
void sendToServer(float temperature, float humidity);

void setup() {
  Serial.begin(9600);
  comSerial.begin(9600);

  // Povezivanje na WiFi mrežu
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Read data from sender
  if (comSerial.available()) {
    recievedString = comSerial.readStringUntil('\n');
    Serial.println(recievedString);

    temperature = parseTemperature(recievedString);
    humidity = parseHumidity(recievedString);

    Serial.print("Parsed Temperature: ");
    Serial.println(temperature);
    Serial.print("Parsed Humidity: ");
    Serial.println(humidity);

    // Slanje podataka na server
    if (!isnan(temperature) && !isnan(humidity)) {
      sendToServer(temperature, humidity);
    }
    
    delay(1000);
  } else {
    Serial.println("Nije procitano");
    delay(1000);
  }
}

void sendToServer(float temperature, float humidity) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");

  String httpRequestData = "{\"temperatura\":\"" + String(temperature) + "\", \"vlaga\":\"" + String(humidity) + "\"}";
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

float parseTemperature(const String& data) {
  int tempIndex = data.indexOf("Temperature: ");
  if (tempIndex != -1) {
    int start = tempIndex + strlen("Temperature: ");
    int end = data.indexOf(" ", start);
    if (end != -1) {
     return data.substring(start, end).toFloat();
    }
  }
  return NAN; // Return NaN if not found
}

float parseHumidity(const String& data) {
  int humIndex = data.indexOf(", Humidity: ");
  if (humIndex != -1) {
    int start = humIndex + strlen(", Humidity: ");
    int end = data.indexOf("%", start);
     if (end != -1) {
     return data.substring(start, end).toFloat();
    }
  }
  return NAN; // Return NaN if not found
}