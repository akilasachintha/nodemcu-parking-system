#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;         // create servo object to control a servo

// WiFi Parameters
const char* ssid = "SkysoFTlk";
const char* password = "@Wushusky1999";
void setup() {
  myservo.attach(0); // attaches the servo on pin 9 to the servo object
  myservo.write(30);
  Wire.begin(4, 5);
  
  lcd.init();
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  lcd.backlight();
  lcd.home();
  lcd.print("Welcome...");
  delay(500);
  lcd.clear();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
    lcd.home();
    lcd.print("Connecting...");
  }
}

WiFiClient client;
const char* uname;
const char* vid;
const char* s_time;
const char* g_status;
int var = 0;

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    if(var == 0){
      lcd.clear();
      lcd.print("Wifi Connected");
      delay(1000);
      lcd.clear(); 
      var = 1; 
    }
    HTTPClient http;  //Object of class HTTPClient

    http.begin(client, "http://smartparkingsystemnsbm.azurewebsites.net/nodemcu/index.php");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      int id = root["id"];  
      uname = root["name"]; 
      vid = root["vid"];
      s_time = root["time"]; 
      g_status = root["status"]; 
 
      
      // Output to serial monitor
      Serial.print("Name:");
      Serial.println(String(uname));
      Serial.print("VID:");
      Serial.println(String(vid));
      Serial.print("Time:"); 
      Serial.println(String(s_time));
      Serial.print("Status:"); 
      Serial.println(g_status);

      lcd.home();
      lcd.print(String(uname));
      lcd.home();
      lcd.print("                ");
      lcd.home();
      lcd.print(String(uname));

      lcd.setCursor(0 ,1);
      lcd.print(String(vid));
      lcd.setCursor(0 ,1);
      lcd.print("       ");
      lcd.setCursor(0 ,1);
      lcd.print(String(vid));
      
      lcd.setCursor(10 ,1);
      lcd.print(String(s_time));
      
      if(String(g_status) == "1"){
        myservo.write(180);  
      }
      else{
        myservo.write(30);  
      }
    }
    http.end();   //Close connection
  }
}
