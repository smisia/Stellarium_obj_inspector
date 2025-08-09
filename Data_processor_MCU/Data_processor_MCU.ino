#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

/*********** Global variable declaraion **************/

const char* ssid = "Your router SSID";
const char* password = "Your router password";
const char* url = "http://loclahost:5000/send";

// HTTP related variables
HTTPClient http;
int httpCode;
String payload;

// Data process related variables
float ra, dec;
char buffer[64];

/*********** End of global variable declaraion ********/

void setup(){

    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    pinMode(2, OUTPUT);
}

void loop(){

  if(WiFi.status() != WL_CONNECTED){
    digitalWrite(2, LOW);
  }else{
    digitalWrite(2, HIGH);
    Serial.println(WiFi.localIP());
  }

  // HTTP commnication
  http.begin(url);

  http.addHeader("Content-Type", "charset=utf-8");

  httpCode = http.GET();

  Serial.printf("HTTP code: %d\n", httpCode);

  if (httpCode > 0) {
            payload = http.getString();
          }else{
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
  http.end();

  // Processing the payload and convering it to usable data
  payload.trim();
  payload.toCharArray(buffer, sizeof(buffer));

  char *token = strtok(buffer, ",");

  if (token != NULL) {
    ra = atof(token);
    token = strtok(NULL, ",");

    if (token != NULL) {
      dec = atof(token);
      Serial.printf("RA = %.5f, DEC = %.5f\n", ra, dec);
    }
  }
    
}
