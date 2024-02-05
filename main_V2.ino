//溫度傳感器for ESP32
#include "max6675.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include "DHT.h"

unsigned long prevTime_T1 = millis();
unsigned long prevTime_T2 = millis();
long interval_T1 = 2000;
long interval_T2 = 6000;

// WiFi credentials
const char* ssid = "YOUR_Wifi";         // change SSID
const char* password = "abc";    // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "ABCD";    // change Gscript ID
String web_url = "https://script.google.com/macros/s/ABCD";
//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
const int maxConnectionAttempts = 2;  // 最大嘗試連線次數
const int retryInterval = 3000;        // 重新連線的時間間隔（毫秒）
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
//----------------------------------------


#define DHTTYPE DHT11 // type of the temperature sensor
const int DHTPin = 5; //--> The pin used for the DHT11 sensor is Pin D1 = GPIO5
DHT dht(DHTPin, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int thermoCLK = 12;
int thermoCS = 14;
int thermoDO = 27;
float temp1 = 0;
char *titleTemp="Temperature: ";
int type=1;// 1=C, 2=F, 3=K,
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
/////////////// end of MAX6675 settings

float th_tem;
float env_tem;
int env_hum;

void setup() {
  Serial.begin(115200);
  delay(100);
  dht.begin();  //--> Start reading DHT11 sensors

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(500);
  display.clearDisplay();
  display.setTextColor(WHITE);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    if (connecting_process_timed_out > 0) {
      connecting_process_timed_out--;
    }
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------
  client.setInsecure();
}

void loop() {


  unsigned long currentTime = millis();

  if (currentTime - prevTime_T1 > interval_T1){
    Getting_Sensor_Data(th_tem, env_tem, env_hum);
    prevTime_T1 = currentTime;
  }

  if (currentTime - prevTime_T2 > interval_T2){
    sendData(th_tem, env_tem, env_hum);
    prevTime_T2 = currentTime;
  }
}

