#include <HTTPClient.h>
#include <WiFiManager.h> 
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>


WiFiClient  client;
WiFiUDP ntpUDP;

const long offsetTime = 25200; 
NTPClient timeClient(ntpUDP, "pool.ntp.org", offsetTime);

String myStatus = "";
String finalresult = "";
String Work = "WW";
String DontWork = "NN";
int year;
int month;
int day;
int hour;
int minute;
int second;
int count =0;
bool set = false;
// หน่วยเป็นวินาที จะได้ 7*60*60 = 25200
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "Base64.h"
#include "esp_camera.h"

// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled

//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

String GOOGLE_SCRIPT_ID = "YOUR ID"; 
String result = "";

const int sendInterval = 5000; 

//updated 04.12.2019
const char * root_ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n" \
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n" \
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n" \
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n" \
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n" \
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n" \
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n" \
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n" \
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n" \
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n" \
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n" \
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n" \
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n" \
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n" \
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n" \
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n" \
"-----END CERTIFICATE-----\n";

void setup() {
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  setupWiFi();
  Serial.println("Serial begin");
  timeClient.begin();
  timeClient.update();
  hour = timeClient.getHours();
  minute = timeClient.getMinutes();
  second = timeClient.getSeconds();
  Serial.println(F("Time Begin : "));
  Serial.println(timeClient.getFormattedTime());
  delay(500);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  delay(10);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  Serial.println("Ready!");
  for(int i = 3; i >= 1; i--){
    Serial.printf("time : %d\n",i);
    delay(1000);
  }

  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 22;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 20;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_XGA);  // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
  s->set_pixformat(s, PIXFORMAT_JPEG);
  s->set_vflip(s, 1); // หมุนภาพ 180 องศา
  s->set_hmirror(s, 1); // สะท้อนภาพแนวนอน
  // s->set_pixformat(s, PIXFORMAT_GRAYSCALE);
  
}

void loop() {
  timeClient.update();
  hour = timeClient.getHours();
  minute = timeClient.getMinutes();
  second = timeClient.getSeconds();
  Serial.print(F("Count : "));
  Serial.println(count);
  if(count > 9) {
    count =0 ;
    set = true;
  }
  getDataFromGoogleSheets();

  String Hash = finalresult;
  Serial.print("This Hash: ");
  Serial.println(Hash.c_str());


  // ใน loop()
String data = "";
if (Hash.equals("WW")) {
  for (int j = 0; j <=1; j++) {
    Photo2Base64();
  }
    Serial.println("Camera Is Working.");
    data = Photo2Base64();
    if (data != "") {
        sendDataToDatabase(data);
        Serial.println(data);
    }
    sendDataW(DontWork, result);
} else if (Hash.equals("NN")) {
    Serial.println("No Orders To Work.");
    // sendDataW(DontWork);
} else {
    Serial.println("Error Charactor.");
    sendDataW(DontWork, "false");
}
    Serial.println("End Process..........");
    digitalWrite(4, LOW);
// delay(1000);

}


void getDataFromGoogleSheets() {
  // คำอธิบาย: ส่วน getDataFromGoogleSheets ไม่มีการเปลี่ยนแปลง
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClientSecure client;
    HTTPClient http;
       //-----------------------------------------------------------------------------------
      client.setInsecure();
      HTTPClient https;
      String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read";
      Serial.println("Reading Data From Google Sheet.....");
      https.begin(client, url.c_str());
      //-----------------------------------------------------------------------------------
      //Removes the error "302 Moved Temporarily Error"
      https.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      //-----------------------------------------------------------------------------------
      //Get the returning HTTP status code
      int httpCode = https.GET();
      Serial.print("HTTP Status Code: ");
      Serial.println(httpCode);
      //-----------------------------------------------------------------------------------
      if(httpCode <= 0){Serial.println("Error on HTTP request"); https.end(); return;}
      //-----------------------------------------------------------------------------------
      //reading data comming from Google Sheet
      String payload = https.getString();
      Serial.println("Payload: "+payload);
      int index1 = payload.indexOf(":");
      if (index1 >= 0) {
          String result = payload.substring(index1 + 2); // ตัดสตริงตั้งแต่ตำแหน่งหลัง ":" ไปจนถึงสุดท้าย
          result.trim(); // ลบช่องว่างหรือการเว้นวรรคที่อาจเหลืออยู่
          Serial.print("Data from Sheet : ");
          Serial.println(result);
          finalresult = result;
      }
      //-----------------------------------------------------------------------------------
      if(httpCode == 200)
      Serial.print("Data from Sheet : ");
      Serial.println(finalresult);
      //-------------------------------------------------------------------------------------
      https.end();

  }
}


String Photo2Base64() {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();  
    if(!fb) {
      Serial.println("Camera capture failed");
      return "";
    }
     

    String imageFile = "data:image/jpeg;base64,";
    char *input = (char *)fb->buf;
    char output[base64_enc_len(3)];
    for (int i=0;i<fb->len;i++) {
      base64_encode(output, (input++), 3);
      if (i%3==0) imageFile += urlencode(String(output));
    }

    esp_camera_fb_return(fb);
    // sendDataToDatabase(imageFile);
    return imageFile;
}

String urlencode(String str) {
  const char *msg = str.c_str();
  const char *hex = "0123456789ABCDEF";
  String encodedMsg = "";
  while (*msg != '\0') {
    if (('a' <= *msg && *msg <= 'z') || ('A' <= *msg && *msg <= 'Z') || ('0' <= *msg && *msg <= '9') || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[(unsigned char)*msg >> 4];
      encodedMsg += hex[*msg & 0xf];
    }
    msg++;
  }
  return encodedMsg;
}

void sendDataToDatabase(String data) {
  // String personId = "000";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    const char* privateKey = "YOUR PRIVATE KEY";
    String currentDateTime = timeClient.getFormattedTime();
    // String payload = "{\"base64\":\"" + data + "\",\"personId\":\"" + personId + "\",\"datetime\":\"" + currentDateTime + "\"}";
    String payload = "{\"base64\":\"" + data + "\",\"personId\":\"NULL\",\"datetime\":\"" + currentDateTime + "\"}";

    // ตั้งค่า HTTP ร้องขอ
    http.begin("https://uts-api.whsse.net/YOUR PATH");
    http.addHeader("private-key", privateKey);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(payload); // ส่ง JSON payload ไปยัง API
    String response = http.getString(); // รับการตอบกลับจาก API
    Serial.print("Request code is : "); // พิมพ์สถานะการร้องขอ HTTP
    Serial.println(httpCode); // พิมพ์สถานะการร้องขอ HTTP
    Serial.print("response Is : "); // พิมพ์สถานะการร้องขอ HTTP
    Serial.println(response); // พิมพ์การตอบกลับ

    // สร้างออบเจ็กต์ JsonDocument เพื่อจัดการกับ JSON response
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response); // แปลง JSON response เป็น JsonDocument
    // เข้าถึงค่า result และเก็บค่าลงในตัวแปร Hash
    bool resultValue = doc["result"];
    Serial.print("Result value is : ");
    Serial.println(resultValue);
    if (resultValue == 1) {
      result = "true";
    } else {
      result = "false";
    }
    http.end(); // จบการเชื่อมต่อ HTTP
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);

    // sendDataR(result);
    Serial.println("result date : " + currentDateTime);
    Serial.println("result send to api : " + result);
    Serial.println("Send Sucess!!");
  } else {
    Serial.println("Send ERROR!");
  }
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
    res = wm.autoConnect("Sensor_CameraIn","12345678"); // password protected ap
    if(!res) {
      Serial.println("Failed to connect");     
    } 
    else {
      Serial.println("connected...:) ");
    }
}