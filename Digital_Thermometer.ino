#include <LiquidCrystal.h>

// LCD module connections
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin assignments for RGB LED
const int redPin = 7;
const int greenPin = 8;
const int bluePin = 9;

// Pin assignment for slide switch
const int switchPin = 6;

// Pin assignment for piezo buzzer
const int buzzerPin = 10;

// Variables
float temperatureCelsius;
bool isFahrenheit = false;
bool buzzerActive = false;

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);

  // Initialize serial communication
  Serial.begin(9600);

  // Set RGB LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Set slide switch pin as input
  pinMode(switchPin, INPUT_PULLUP);

  // Set piezo buzzer pin as output
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Read slide switch state
  int switchState = digitalRead(switchPin);

  // Toggle between Celsius and Fahrenheit
  if (switchState == LOW) {
    isFahrenheit = !isFahrenheit;
    delay(200);
  }

  // Read temperature from TMP36
  int sensorValue = analogRead(A0);
  temperatureCelsius = (sensorValue * 0.004882814) * 100.0;

  // Convert to Fahrenheit if required
  float temperature = isFahrenheit ? (temperatureCelsius * 1.8) + 32 : temperatureCelsius;

  // Display temperature on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);

  // Display the temperature unit based on the switch state
  lcd.print(isFahrenheit ? "F " : "C ");

  // Set RGB LED color based on temperature gradient
  setGradientColor(temperatureCelsius);

  // Check if temperature is extremely hot and activate the piezo buzzer
  if (temperatureCelsius > 60) {
    if (!buzzerActive) {
      activateBuzzer();
      buzzerActive = true;
    }
  } else {
    if (buzzerActive) {
      stopBuzzer();
      buzzerActive = false;
    }
  }

  // Print temperature to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(isFahrenheit ? "F" : "C");

  delay(500);
}

// Function to set RGB LED color based on temperature gradient
void setGradientColor(float temperature) {
  // Map temperature to a color gradient range (e.g., blue to red)
  int minTemperature = 10; // Minimum temperature in gradient range
  int maxTemperature = 40; // Maximum temperature in gradient range

  // Map temperature value to a gradient range (0-255)
  int colorValue = map(temperature, minTemperature, maxTemperature, 0, 255);

  // Set the RGB LED color using the gradient value
  analogWrite(redPin, colorValue);
  analogWrite(greenPin, 255 - colorValue);
  analogWrite(bluePin, 0);
}

// Function to activate the piezo buzzer
void activateBuzzer() {
  tone(buzzerPin, 1000); // Set the buzzer frequency to 1000 Hz
}

// Function to stop the piezo buzzer
void stopBuzzer() {
  noTone(buzzerPin); // Turn off the buzzer
}
