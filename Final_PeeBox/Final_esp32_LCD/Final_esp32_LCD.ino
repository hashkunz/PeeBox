#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h> 
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <HTTPClient.h>

// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient  client;
WiFiUDP ntpUDP;

const long offsetTime = 25200; 
NTPClient timeClient(ntpUDP, "pool.ntp.org", offsetTime);

String myStatus = "";
int year;
int month;
int day;
int hour;
int minute;
int second;
int count =0;
bool set = false;
// หน่วยเป็นวินาที จะได้ 7*60*60 = 25200

// const int vout = 32;
int buttonstate = 1;
String finalresult = "";
String GOOGLE_SCRIPT_ID = "YOUR ID";
String DontWork = "NN";
String Work = "WW";

void setup() {
  Serial.begin(115200);
  setupWiFi();
  // pinMode(34, INPUT);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("SYSTEM RAEDY!!");
  lcd.setCursor(0, 1);
  lcd.print("< Press Button");
}
  

void loop() {
  // buttonstate = digitalRead(34);
  if (buttonstate == 0){
    Serial.printf("This Logic : %d\n",buttonstate);
    delay(3000);
    lcd.setCursor(1, 0);
    lcd.print("SYSTEM RAEDY!!");
    lcd.setCursor(0, 1);
    lcd.print(" Press Button >");
  } else {
      Serial.printf("This Logic : %d\n",buttonstate);
      lcd.setCursor(5, 0);
      lcd.print("Welcome!");
      lcd.setCursor(2, 1);
      lcd.print("SYSTEM RAEDY!!");
      lcd.clear();
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.print("Place Pee box!!");
      for (int i = 10; i >= 0; i--) {
        lcd.setCursor(3, 1);
        if ( i == 9) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Place Pee box!!");
          lcd.setCursor(3, 1);
        }
        lcd.printf("Wait : %d s",i);
        Serial.printf("Time : %d s\n",i);
        delay(1000);
      }

      delay(3000);
      Serial.println("Run Function cam");
      cam();
    }
}

void cam(){
  lcd.begin();
  lcd.backlight();
  sendDataW(Work,"");
  for(int i = 0; i<1; i++){
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Taking pictures");
      for (int i = 20; i >= 0; i--) {
        lcd.setCursor(3, 1);
        if ( i == 9) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Taking pictures");
          lcd.setCursor(3, 1);
        }
        lcd.printf("Wait : %d s",i);
        Serial.printf("Time : %d s\n",i);
        delay(1000);
      }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Finished Sending.");
  lcd.setCursor(4, 1);
  lcd.print("Bye Bye!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Result Is");
  lcd.setCursor(6, 1);
  lcd.print("True");
  delay(2000);
  // sendDataW(DontWork,"");
}


void sendDataW(String params1, String params2) {
  WiFiClientSecure client;
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec";
  Serial.print("Making a request to: ");
  Serial.println(url);

  // Add your request data here
  String postData = "params1=" + params1 + "&params2=" + params2;
  client.setInsecure();
  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  Serial.print("HTTP Response code: ");
  Serial.println(httpCode);

  http.end();
}


void setupWiFi() {
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("Sensor_Main","12345678"); // password protected ap
    if(!res) {
      Serial.println("Failed to connect");     
    } 
    else {
      Serial.println("connected...:) ");
    }
}