#include <WiFi.h>

const char* ssid = "No_Network";           // Replace with your WiFi network name
const char* password = "rifat2001005";     // Replace with your WiFi password
const char* serverIP = "192.168.156.155";      // IP address of the receiver ESP32
const int serverPort = 80;                  // Port number

const int buttonPin = 21;                   // GPIO pin for the push button
bool ledState = false;                       // State of the LED (true for ON, false for OFF)
bool lastButtonState = HIGH;                 // Variable to hold the last button state

WiFiClient client;

void setup() {
  Serial.begin(115200);
  
  // Configure the button pin with internal pull-up
  pinMode(buttonPin, INPUT_PULLUP);         // Set the button pin as input with pull-up
  
  WiFi.begin(ssid, password);                // Connect to Wi-Fi

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int reading = digitalRead(buttonPin);     // Read the button state

  // Only proceed if there is a valid reading
  if (reading != -1) { // Valid reading check
    // Check if the button state has changed
    if (reading == LOW && lastButtonState == HIGH) { // Button pressed (active low)
      ledState = !ledState;                   // Toggle LED state
      int valueToSend = ledState ? 1 : 0;     // Determine value to send
      Serial.print("Button pressed! Sending ");
      Serial.println(valueToSend);
      sendCommand(valueToSend);                // Send the current value to the receiver
      delay(200);                              // Delay to prevent bouncing
    }

    lastButtonState = reading;                 // Save the reading for the next loop
  }
}

void sendCommand(int value) {
  if (client.connect(serverIP, serverPort)) { // Attempt to connect to the server
    // Send data as an integer
    client.print(value);                       // Send the button state
    Serial.print("Sent: ");
    Serial.println(value);
    
    // Wait for the response
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.print("Response from receiver: ");
      Serial.println(response);
    }
    client.stop();                             // Close the connection
  } else {
    Serial.println("Connection to receiver failed.");
  }
}
