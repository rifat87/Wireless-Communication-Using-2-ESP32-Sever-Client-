#include <WiFi.h>

const char* ssid = "No_Network";           // Replace with your WiFi network name
const char* password = "rifat2001005";     // Replace with your WiFi password

WiFiServer server(80);                      // Create a server that listens on port 80
const int ledPin = 2;                       // GPIO pin for the LED

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);                // Ensure the LED is off initially

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();                            // Start the server
}

void loop() {
  WiFiClient client = server.available();    // Check for incoming clients

  if (client) {                              // If a client connects
    Serial.println("New Client connected.");

    // Read the incoming data
    String request = client.readStringUntil('\n');
    Serial.print("Received data: ");
    Serial.println(request); // Print the received data for debugging

    // Convert received string to integer
    int value = request.toInt();              // Convert string to integer

    // Turn the LED based on the received value
    if (value == 1) {
      digitalWrite(ledPin, HIGH);            // Turn the LED on
      Serial.println("LED ON");
    } else if (value == 0) {
      digitalWrite(ledPin, LOW);             // Turn the LED off
      Serial.println("LED OFF");
    }

    // Send HTTP response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println();                         // End of HTTP response

    client.stop();                           // Close the connection
    Serial.println("Client Disconnected.");
  }
}
