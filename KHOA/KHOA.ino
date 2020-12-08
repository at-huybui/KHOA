#include "RC522.h"
#include "LCD.h"
#include "DEFINE.h"

#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>

#include <SoftwareSerial.h>

#define WLAN_SSID "CHAU_CHAU"          //enter your WiFi network name within the double quotes & 
#define WLAN_PASS "14022019"          //password here

#define AIO_SERVER       "io.adafruit.com"
#define AIO_SERVERPORT   1883               // use 8883 is used for SSL

#define AIO_USERNAME  "anhkhoa2551"
#define AIO_KEY       "aio_NcoK49XQxjQ28H0J6hqr6AKBnwYQ"

void ICACHE_RAM_ATTR ISR_vibrate();

unsigned long mil_connect_mqtt = 0;
unsigned long mil_scan_card = 0;            // Biến millis hẹn giờ quét card
unsigned long mil_check_vibrate_sensor = 0; // Biến millis hẹn giờ check sensor
unsigned long mil_unlock = 0;               // Biến millis hẹn thời gian mở khóa
unsigned long mil_turnLcd = 0;               // Biến millis hẹn giờ tắt lcd
unsigned long mil_checkSMS = 0;               // Biến millis hẹn giờ check SMS
unsigned long mil_runloop = 0;               // Biến millis runloop

WiFiClient client;                                                                                  // Create an ESP8266 WiFiClient class to connect to the MQTT server.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);              // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Subscribe LockSubscribe = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lock");  // Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Publish LockPublish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lock");
Adafruit_MQTT_Publish WarningPublish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/warning");

byte state = 0;
bool state_wifi = false;

volatile bool _isSending = true;   // lưu trạng thái đang gửi tin nhắn
volatile bool _isLock = false;     // Biến lưu trạng thái cửa
bool _isTurnLcd = false;           // trạng thái bật lcd

volatile static byte _countVibrate = 0;   // biến lưu số lần ngắt tạo rung động

void setup() {

  //  Serial.begin(9600); // Mở cổng serial debug
  System_init();      // khởi tạo hệ thông
  Lcd_init();         //Khởi tạo LCD
  Lcd_print("---CONNECTING---", 0, 0);
  Serial.println("OK connecting");
  RF_init();
  delay(1000);
  state_wifi = init_WIFI();
  init_MQTT();
  attachInterrupt(digitalPinToInterrupt(PIN_VIBRATE), ISR_vibrate, FALLING); // Bật ngắt cho chân cảm biến rung
  Lcd_print(F("----WELLCOME----"), 0, 0);
  doubleTick();  // còi báo
  onLcd();
  _isSending = false;
}


void loop() {
  if (!_isSending) {
    scanCard();
    checkVibrate();
    smsUnLock();
    MQTT_connect();
  }
}

void scanCard() {
  if (_isLock ) { //kiểm tra thời gian quét và trạng thái khóa
    String _id = RF_getID();                     //quét thẻ
    if (RF_matchId(_id)) {
      Serial.print(F("[id]")); Serial.println(_id);
      _unLock();
    }
  }

  if (isOpen()) {
    if (_isLock) {
      _unLock();
    }
    mil_unlock = millis();
  }

  if (millis() - mil_unlock > TIME_UNLOCK && !_isLock && !isOpen()) { // kiêm tra biến hẹn giờ và trạng thái khóa
    onLcd();
    doubleTick();
    lock();  // khóa
    if (state_wifi) {
      LockPublish.publish("OFF");
    }
    Lcd_print(F("------Lock------"), 0, 1);
    Serial.println(F("------Lock------"));
    _isLock = true; // set lại trạng thái khóa
  }

  if (_isTurnLcd && millis() - mil_turnLcd > 3000) {
    Lcd_noLight();
  }
}

/* Mở Khóa */
void _unLock() {
  _isLock = false;
  onLcd();
  unLock();
  tick();
  _countVibrate = 0;
  Lcd_print(F("-----Unlock-----"), 0, 1);
  mil_unlock = millis(); // set lại biến hẹn giờ
  Serial.println(F("-----Unlock-----"));
}

/* bật tắt led LCD */
void onLcd() {
  _isTurnLcd = true;
  Lcd_light();
  mil_turnLcd = millis();
}

/*kiểm tra số lần rung động của cảm biến rung */
void checkVibrate() {
  if (_countVibrate > 50) {
    if (!isOpen() && _isLock) {
      Serial.println(F("*****************WARNING*******************"));
      if (sendSMS("**WARNING**")) {
        Serial.println(F("Đã send Warning"));
        for (int i = 0; i < 10; i++) {
          doubleTick();
        }
      } else {
        Serial.println(F("Chưa send Warning"));
      }
      _countVibrate = 0;
    } else {
      _countVibrate = 0;
    }
  }
}

/* Kiếm tra SMS để mở khóa */
void smsUnLock() {
  if (millis() - mil_checkSMS > 2000 && state_wifi) {
    mil_checkSMS = millis();
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(1)))
    {
      if (subscription == &LockSubscribe) {
        Serial.print(F("Got: "));
        Serial.println((char *)LockSubscribe.lastread);
        String response = (char*)LockSubscribe.lastread; //converts the received 1 or 0 to string to compare in the if-else statement
        if (response == "ON" || response == "1")
        {
          _unLock();
          mil_unlock = millis(); // set lại biến hẹn giờ
        }
      }
    }
  }
}

/*gửi SMS */
bool sendSMS(char* msg) {
  onLcd();
  if (state_wifi) {
    _isSending = true;
    while (! WarningPublish.publish("ON")) {
      Serial.println(F("Publish ON"));
    }
    delay(2000);
    while (! WarningPublish.publish("OFF")) {
      Serial.println(F("Publish OFF "));
    }
    Serial.println(F("Publish OK"));
    _isSending = false;
    return true;
  }
  return false;
}

bool init_WIFI() {
  int point = 0;
  Serial.println();
  Serial.print(F("Connecting to "));       // Connect to WiFi access point.
  Serial.println(WLAN_SSID);
  for (int j = 1; j < 4; j++) {
    Serial.print(F("\nTime ")); Serial.print(j);  Serial.println(F(" ------"));
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    for (int i = 0; WiFi.status() != WL_CONNECTED && i <= 40; i++)
    {
      delay(500);
      if (point == 0) {
        Lcd_clearY(1);
      }
      Lcd_print(".", point, 1);
      point++;
      if (point == 10)   point = 0;
      if (i % 10 != 0) {
        Serial.print(".");
      } else {
        Serial.print(i);
      }

    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println(F("\WiFi connected"));
      Serial.print(F("IP address: "));
      Serial.println(WiFi.localIP());
      return true;
    }
  }
  Serial.println(F("\WiFi not connected"));
  return false;
}

void init_MQTT() {
  if (state_wifi) {
    mqtt.subscribe(&LockSubscribe);        // Setup MQTT subscription for onoff feed.
    MQTT_connect();
  }
}


/* Kiểm tra kết nối với mqtt*/
bool MQTT_connect()
{
  if (millis() - mil_connect_mqtt > 3000 && state_wifi) {
    mil_connect_mqtt = millis();
    int8_t ret;
    if (mqtt.connected())     // Stop if already connected.
    {
      return false;
    }
    Serial.print(F("Connecting to MQTT... "));
    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0)
    {
      Serial.println(mqtt.connectErrorString(ret));                  // connect will return 0 for connected
      Serial.println(F("Retrying MQTT connection in 5 seconds..."));
      mqtt.disconnect();
      delay(5000);              // wait 5 seconds
      retries--;
      if (retries == 0)
      {
        while (1);              // basically die and wait for WDT to reset me
      }
    }
    Serial.println(F("MQTT Connected!"));
    return true;
  }
}


/* Hàm ngắt tại chân số D2, ngắt cho cảm biến rung */
void ISR_vibrate() {
  if (_isLock && !_isSending) {
    _countVibrate++;
    Serial.println(_countVibrate);
  }
}
