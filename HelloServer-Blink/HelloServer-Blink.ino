#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "IoT Hub";
const char* password = "letsbuildsomething";

// initialize server port (standard HTTP)
ESP8266WebServer server(80);

const int LED = D5;
boolean state = false;

void setup(void) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  // sets the mode to "STAtion" mode (server)
  // vs Access Point (AP) mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // request to root url
  server.on("/", handleRoot);

  // request to /inline url (path)
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

void handleRoot() {
  String message = "Hello from ESP8266!\n";
  message.concat("Refresh the browser page to toggle LED.\n");
  message.concat("LED is: ");
  message.concat((state) ? "on": "off");
  server.send(200, "text/plain", message);
  digitalWrite(LED, state);
  state = (state) ? false : true;
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
  digitalWrite(LED, LOW);
}
