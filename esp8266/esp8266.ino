#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "HelloWorld";
const char* password = "Aa123456";
const char* mqttServer = "";//服务器地址：xxx.com
const char* addtimeServer = "";//服务器地址：xxx.com
const int addtimePort = 8080;//服务器端口号
String s;                           //临时存储接收的数据
String result;
String way;
WiFiClient wifiClient;
WiFiClient addTime;
PubSubClient mqttClient(wifiClient);

int subQoS = 0;
bool cleanSession = true;
//遗嘱消息
const char* willTopic = "willTopic";
const char* willMsg = "offline";
bool willRetain = false;
int willQoS = 0;
String reqRes = "/AddTime?result=" + result + "&way=" + way;
String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                     "Host: " + addtimeServer + "\r\n" +
                     "Connection: close\r\n\r\n";
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  connectWiFi();
  //addTime.connect(addtimeServer, 8080);
  //addTime.print(httpRequest);
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setKeepAlive(60);
  mqttClient.setCallback(receiveCallback);
  connectMQTTserver();
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTTserver();
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
  digitalWrite(LED_BUILTIN, LOW);
  serialEvent();
  mqttClient.loop();
}

void connectMQTTserver() {
  String clientId = "ESP8266";
  if (mqttClient.connect(clientId.c_str(), willTopic, willQoS, willRetain, willMsg)) {
    publishOnlineStatus();
    subscribeTopic();
  }
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }
}
//回调函数
void receiveCallback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message Received [");
  //Serial.print(topic);
  //Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}
void pubMQTTmsg(String LockState) {     //发布信息
  static int value; // 客户端发布信息用数字
  String topicString = "LockState";
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic, topicString.c_str());

  String messageString = LockState;
  char publishMsg[messageString.length() + 1];
  strcpy(publishMsg, messageString.c_str());

  if (mqttClient.publish(publishTopic, publishMsg,true)) {
  }
}

void pubMQTTmsg1(String LockorUnlock) {     //发布信息
  static int value; // 客户端发布信息用数字
  String topicString = "LockorUnlock";
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic, topicString.c_str());

  String messageString = LockorUnlock;
  char publishMsg[messageString.length() + 1];
  strcpy(publishMsg, messageString.c_str());

  if (mqttClient.publish(publishTopic, publishMsg,true)) {
  }
}
void pubMQTTmsg2(String TH) {     //发布信息
  static int value; // 客户端发布信息用数字
  String topicString = "TH";
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic, topicString.c_str());

  String messageString = TH;
  char publishMsg[messageString.length() + 1];
  strcpy(publishMsg, messageString.c_str());

  if (mqttClient.publish(publishTopic, publishMsg,true)) {
  }
}

void publishOnlineStatus() {
  String willString = "willTopic";
  char willTopic[willString.length() + 1];
  strcpy(willTopic, willString.c_str());

  String onlineMessageString = "online";
  char onlineMsg[onlineMessageString.length() + 1];
  strcpy(onlineMsg, onlineMessageString.c_str());

  if (mqttClient.publish(willTopic, onlineMsg, true)) {
  }
}

void subscribeTopic() {
  String topicString = "LockControl";
  char subTopic[topicString.length() + 1];
  strcpy(subTopic, topicString.c_str());

  if (mqttClient.subscribe(subTopic, subQoS)) {
  }
}

void serialEvent() {
  if (Serial.available() > 0) {
    s = Serial.readString();
    
    char TH[s.length() + 1]; //温湿度
    strcpy(TH, s.c_str());
    
    if (s.equals("fingeropen\r\n")) {
      WiFiClient addTime;
      result = "success";
      way = "finger";
      reqRes = "/AddTime?result=" + result + "&way=" + way;
      httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                    "Host: " + addtimeServer + "\r\n" +
                    "Connection: close\r\n\r\n";
      addTime.connect(addtimeServer, 8080);
      addTime.print(httpRequest);
    }
    if (s.equals("fingerfailed\r\n")) {
      WiFiClient addTime;
      result = "failure";
      way = "finger";
      reqRes = "/AddTime?result=" + result + "&way=" + way;
      httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                    "Host: " + addtimeServer + "\r\n" +
                    "Connection: close\r\n\r\n";
      addTime.connect(addtimeServer, 8080);
      addTime.print(httpRequest);
    }
    if (s.equals("cloudopen\r\n")) {
      WiFiClient addTime;
      result = "success";
      way = "cloud";
      reqRes = "/AddTime?result=" + result + "&way=" + way;
      httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                    "Host: " + addtimeServer + "\r\n" +
                    "Connection: close\r\n\r\n";
      addTime.connect(addtimeServer, 8080);
      addTime.print(httpRequest);
    }
    if (s.equals("webopen\r\n")) {
      WiFiClient addTime;
      result = "success";
      way = "web";
      reqRes = "/AddTime?result=" + result + "&way=" + way;
      httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                    "Host: " + addtimeServer + "\r\n" +
                    "Connection: close\r\n\r\n";
      addTime.connect(addtimeServer, 8080);
      addTime.print(httpRequest);
    }
    if (s.equals("P0_7=0\r\n")) {
      String state = "close";
      pubMQTTmsg(state);
    }
    if (s.equals("P0_7=1\r\n")) {
      String state = "open";
      pubMQTTmsg(state);
    }
    if (s.equals("lock\r\n")) {
      String state = "lock";
      pubMQTTmsg1(state);
    }
    if (s.equals("unlock\r\n")) {
      String state = "unlock";
      pubMQTTmsg1(state);
    }
    if(TH[0]=='T' && TH[1]=='H'){
      Serial.println(s);
      pubMQTTmsg2(s);
    }
  }
}
