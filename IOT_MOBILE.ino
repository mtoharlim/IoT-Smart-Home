// Blynk + DHT11 Monitoring with NodeMCU
#define BLYNK_TEMPLATE_ID "TMPL69Yz0KdUA"
#define BLYNK_TEMPLATE_NAME "ESP32 MQTT"
#define BLYNK_AUTH_TOKEN "KPfQhfuHOFKuOzJkFIn1pIaw904LrjmP"

#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>

// Pins configuration
#define DHTPIN D3       // DHT11 connected to pin D3
#define DHTTYPE DHT11   // DHT11 sensor type
#define MQ2PIN D2       // MQ2 Gas sensor connected to analog pin
#define DOORPIN D5      // Door sensor connected to pin D5
#define LAMP1PIN D6     // Lamp 1 connected to pin D6
#define LAMP2PIN D0     // Lamp 2 connected to pin D0

// WiFi credentials
char ssid[] = "apahayo";
char pass[] = "passwordnyata";

// Initialize components
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Function to read sensor and send data to Blynk
void sendSensorData() {
  // DHT11 Readings
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  // Check if DHT reading failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // Log to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Send data to Blynk
    Blynk.virtualWrite(V0, temperature);  // Virtual pin V0 for temperature
    Blynk.virtualWrite(V1, humidity);     // Virtual pin V1 for humidity
  }

  // MQ2 Gas Sensor Reading
  int gasValue = analogRead(MQ2PIN);
  Serial.print("Gas value: ");
  Serial.println(gasValue);
  Blynk.virtualWrite(V2, gasValue);  // Virtual pin V2 for gas value

  // Door Sensor Reading
  int doorStatus = digitalRead(DOORPIN);
  String doorState = doorStatus ? "Open" : "Closed";
  Serial.print("Door: ");
  Serial.println(doorState);
  Blynk.virtualWrite(V3, doorState);  // Virtual pin V3 for door status
}

// Lamp 1 control from Blynk
BLYNK_WRITE(V5) {
  int lamp1State = param.asInt();  // Get value from the button
  digitalWrite(LAMP1PIN, lamp1State);
  Serial.print("Lamp 1: ");
  Serial.println(lamp1State ? "ON" : "OFF");
}

// Lamp 2 control from Blynk
BLYNK_WRITE(V6) {
  int lamp2State = param.asInt();  // Get value from the button
  digitalWrite(LAMP2PIN, lamp2State);
  Serial.print("Lamp 2: ");
  Serial.println(lamp2State ? "ON" : "OFF");
}

// run servo
const int servoPin = D8; // You can change this to your desired digital pin
Servo myServo;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(MQ2PIN, INPUT);
  pinMode(DOORPIN, INPUT_PULLUP);  // Use pull-up for door sensor
  pinMode(LAMP1PIN, OUTPUT);
  pinMode(LAMP2PIN, OUTPUT);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup functions to be called on a timer
  timer.setInterval(2000L, sendSensorData);

  // setting servo
  myServo.attach(servoPin); // Attaches the servo on the digital pin to the servo object
  myServo.write(90); 
  
  Serial.println("System initialized");
}

void loop() {
  Blynk.run();  // Run Blynk
  timer.run();  // Run timer
 
}

// moving servo
BLYNK_WRITE(V7) {
  int pos = param.asInt(); // Get integer value from Blynk app
  Serial.print("Moving servo to: ");
  Serial.println(pos);
  myServo.write(pos); 
}
