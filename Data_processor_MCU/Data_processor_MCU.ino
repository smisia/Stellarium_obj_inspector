#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

/*********** Global variable declaraion **************/
#define INTERNAL_LED 2
#define BUTTON_PIN 5
#define WAKEUP_GPIO GPIO_NUM_5

const char* ssid = "Your router's ssid";
const char* password = "Your router's password";
const char* url = "http://loclahost:5000/send";

volatile bool button_pressed = false;

// HTTP related variables
HTTPClient http;
String payload;

// Data process related variables
float ra, dec;
char buffer[64];

/*********** End of global variable declaraion ********/


/*********** Function prototype declarations **********/

// Processing the payload and convering it to usable data
void process_data(char* buffer);

// Executes the whole http process
void handle_http(const char* input_url, String input_payload);

void IRAM_ATTR handleButtonInterrupt();

int read_button(int input_pin);

/****** End of function prototype declarations ********/


void setup(){
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(INTERNAL_LED, OUTPUT);

    esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, HIGH);

    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

void loop(){

  if(WiFi.status() != WL_CONNECTED){
    digitalWrite(2, LOW);
  }else{
    digitalWrite(2, HIGH);
    Serial.println(WiFi.localIP());
  }

  handle_http(url,payload);

  // Temporary solution until button functionality is not implemented
  if(button_pressed){
    button_pressed = false;
    Serial.println("Button is pressed!");
  }

  process_data(buffer);
}

/****************** Functions ********************/

void handle_http(const char* input_url, String input_payload){
  
  http.begin(input_url);
  http.addHeader("Content-Type", "charset=utf-8");
  int httpCode = http.GET();
  Serial.printf("HTTP code: %d\n", httpCode);

  if (httpCode > 0) {
            input_payload = http.getString();
          }else{
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
  http.end();
}

/*************************************************/
void process_data(char* buffer){

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

/*************************************************/
void IRAM_ATTR handleButtonInterrupt(){
    esp_deep_sleep_start();
}

/*************************************************/
int read_button(int input_pin){
  int buttonState = digitalRead(input_pin);
  return buttonState;
}
/*************** End of functions ***************/