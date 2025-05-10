#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "abuammar2";
const char* password = "abuammar2017";

// Set Static IP Address
IPAddress local_IP (192, 168, 100, 200);  // Choose an available IP in your network
IPAddress gateway(192, 168, 100, 1);    // Router's IP (Check your network settings)
IPAddress subnet(255, 255, 0, 0); // for both // Subnet mask
IPAddress dns(192, 168, 100, 1);         // DNS server (same as the gateway in most cases)

ESP8266WebServer server(80);
const int ledPin = D1;  // LED connected to GPIO5

bool ledState = false;

void toggleLED() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? HIGH : LOW);
  server.send(200, "text/plain", ledState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi with Static IP
  WiFi.config(local_IP, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("Static IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/toggle", toggleLED);
  server.begin();
}

void loop() {
  server.handleClient();
}
