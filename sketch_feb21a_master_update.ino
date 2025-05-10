#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "abuammar2";
const char* password = "abuammar2017";

// Set Static IP for Master
IPAddress local_IP(192, 168, 100, 101);  // Choose an available IP in your network
IPAddress gateway(192, 168, 100, 1);     // Router's IP
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 100, 1);

// Static IP of the Slave ESP8266
const char* slaveIP = "192.168.100.200"; 

ESP8266WebServer server(80);
const int ledPin = D1;  // LED connected to GPIO5

bool ledStateMaster = false;
bool ledStateSlave = false;

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<style>";
  html += "body { text-align: center; font-family: Arial, sans-serif; }";
  html += "h2 { color: black; margin-top: 20px; }";
  html += ".button { display: block; width: 200px; height: 60px; font-size: 20px; ";
  html += "border: none; border-radius: 10px; margin: 20px auto; cursor: pointer; ";
  html += "text-align: center; line-height: 60px; text-decoration: none; font-weight: bold; text-transform: uppercase;}";
  html += ".blue { background-color: lightblue; color: black; }";
  html += ".green { background-color: lightgreen; color: black; }";
  html += "</style>";
  html += "</head><body>";
  
  html += "<h2>ESP8266 LED Control</h2>";
  
  html += "<p>Master LED: " + String(ledStateMaster ? "ON" : "OFF") + "</p>";
  html += "<a href='/toggleMaster' class='button blue'>MASTER LED</a>";
  
  html += "<p>Slave LED: " + String(ledStateSlave ? "ON" : "OFF") + "</p>";
  html += "<a href='/toggleSlave' class='button green'>SLAVE LED</a>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void toggleMaster() {
  ledStateMaster = !ledStateMaster;
  digitalWrite(ledPin, ledStateMaster ? HIGH : LOW);
  handleRoot();
}

void toggleSlave() {
  WiFiClient client;
  if (client.connect(slaveIP, 80)) {
    client.print("GET /toggle HTTP/1.1\r\nHost: " + String(slaveIP) + "\r\nConnection: close\r\n\r\n");
    delay(500);
    
    String response = "";
    while (client.available()) {
      response += client.readString();
    }
    client.stop();
    
    if (response.indexOf("ON") != -1) {
      ledStateSlave = true;
    } else if (response.indexOf("OFF") != -1) {
      ledStateSlave = false;
    }
  }
  handleRoot();
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi with Static IP
  WiFi.config(local_IP, gateway, subnet, dns);
  WiFi.begin(ssid, password);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("Static IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggleMaster", toggleMaster);
  server.on("/toggleSlave", toggleSlave);

  server.begin();
}

void loop() {
  server.handleClient();
}
