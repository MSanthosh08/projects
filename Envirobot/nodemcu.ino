#include <ESP8266WiFi.h>
#include <DHT.h>

// Replace with your network credentials
const char *ssid = "OPPOF21PRO";
const char *password = "12345678";

#define DHTPIN D2        // Pin where the DHT sensor is connected
#define DHTTYPE DHT11    // DHT 22 (AM2302) sensor type
DHT dht(DHTPIN, DHTTYPE);

// Create an instance of the server
WiFiServer server(80);

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  dht.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while (!client.available()) {}

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Send HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<html><body>");
  client.println("<h1>Real-Time Temperature and Humidity</h1>");
  client.println("<p>Temperature: " + String(temperature) + " &#8451;</p>");
  client.println("<p>Humidity: " + String(humidity) + " %</p>");
  client.println("<p><a href='/manual'>Manual Control</a></p>");
  client.println("<p><a href='/autonomous'>Autonomous Mode</a></p>");
  client.println("</body></html>");

  if (request.indexOf("/manual") != -1) {
    // Handle manual control, serve HTML with manual control buttons
    serveManualControlPage(client);
  } else if (request.indexOf("/autonomous") != -1) {
    // Handle autonomous control, serve HTML with autonomous control buttons
    serveAutonomousControlPage(client);
  } 

  delay(500);
  client.stop();
  Serial.println("Client disconnected");
}

void serveManualControlPage(WiFiClient client) {
    String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<html><body>");
  client.println("<h1>Manual Control</h1>");
  client.println("<p><a href='/forward'>Forward</a></p>");
  client.println("<p><a href='/backward'>Backward</a></p>");
  client.println("<p><a href='/right'>Right</a></p>");
  client.println("<p><a href='/left'>Left</a></p>");
  client.println("</body></html>");
  if (request.indexOf("/forward") != -1) {
    // Send command to Arduino for forward motion
    Serial.println("F");
    // Add code here to control the robot's forward motion
  } else if (request.indexOf("/backward") != -1) {
    // Send command to Arduino for backward motion
    Serial.println("B");
    // Add code here to control the robot's backward motion
  } else if (request.indexOf("/right") != -1) {
    // Send command to Arduino for right turn
    Serial.println("R");
    // Add code here to control the robot's right turn
  } else if (request.indexOf("/left") != -1) {
    // Send command to Arduino for left turn
    Serial.println("L");
    // Add code here to control the robot's left turn
  }

  delay(500);
  client.stop();
  Serial.println("Client disconnected");
}

void serveAutonomousControlPage(WiFiClient client) {
  // Add code for serving autonomous control HTML page
  // This can include buttons or links for autonomous control
  // For simplicity, I'm just serving a placeholder message here
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<html><body>");
  client.println("<h1>Autonomous Control</h1>");
  client.println("<p>This is a placeholder for autonomous control.</p>");
  client.println("</body></html>");

  delay(500);
  client.stop();
  Serial.println("Client disconnected");
}
