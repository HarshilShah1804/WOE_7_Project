#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
using namespace std;

const int COMM = 23;

const char* ssid = "wifi_name";
const char* password = "wifi_password";

String serverName = "server_url";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
int isOn = 0;

void setup() {
  Serial.begin(115200); 
  pinMode(COMM, INPUT);
  while (!Serial);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

bool send = 0;

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      String serverPath = serverName;

      http.begin(serverPath.c_str());
      http.addHeader("Content-Type", "application/json");
      std::string body = "{\"rssi_value\": " + to_string(isOn) + "}";
      // string body = "{\"rssi_value\": ";
      // body += "\"Something\"";
      // body += "}";
      int httpResponseCode = http.POST(body.c_str());
        // int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
    if (digitalRead(COMM) == HIGH) {
    Serial.println("Baby in");
    isOn = 1;
  } else {
    isOn = 0;
    Serial.println("Baby out");
  }
  }

  
}