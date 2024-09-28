#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>


const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = ""; 

const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "techadvanced_mqt";
const char *TOPIC_MSG = "fiap/techadvanced/msg";
const char *TOPIC_PUBLISH_TEMP = "fiap/techadvanced/temp";

#define DS_18B20 32


#define PUBLISH_DELAY 2000
const int TAMANHO = 200;
LiquidCrystal_I2C lcd(0x27, 20, 4);  
WiFiClient espClient;
PubSubClient MQTT(espClient);
OneWire one_wire(DS_18B20);


DallasTemperature sensor(&one_wire);

int num_sensor;
float temp;

void initWiFi() {
  Serial.print("Conectando com a rede:");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg = String((char*)payload).substring(0, length);
  
  Serial.printf("Mensagem recebida via MQTT: %s do tópico: %s\n", msg.c_str(), topic);

  if (strcmp(topic, TOPIC_MSG) == 0) {
    String valor = msg.c_str();
    if (msg.length() < 20){
     lcd.setCursor(0,2);
     lcd.print(msg.c_str());
    }
    else{
     lcd.setCursor(0,2);
     lcd.print(msg.substring(0,19).c_str());
     lcd.setCursor(0,3);
     lcd.print(msg.substring(20,39).c_str());
    }


}}


void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT:");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
      MQTT.subscribe(TOPIC_MSG);
    } else {
      Serial.println("Falha na conexã com MQTT. Tentando novamente em 2 segundos. ");
      delay(2000);
    }  
}}

void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wifi conectado com sucesso");
  Serial.print(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}


void setup()
{
 Serial.begin(115200);
 lcd.init();
 lcd.backlight();
 initWiFi();
 initMQTT();
 delay(2000);
   
 sensor.begin();
 
 Serial.println("Procurando sensores ...");

 num_sensor = sensor.getDeviceCount();

 if(num_sensor >0)
 {
   Serial.print("Encontrou  ");
   Serial.print(num_sensor);
   Serial.println(" sensores");
 }
 
 else
 {
   Serial.println(" Não encontrou o(s) sensor");
   return;
 }
}
void loop() {
      checkWiFIAndMQTT();
      sensor.requestTemperatures(); 
      MQTT.loop();
      temp = sensor.getTempCByIndex(0); 

      StaticJsonDocument<TAMANHO> json;
      json["temperatura"] = temp;

      if (temp > 40 && temp < 80){
        lcd.setCursor(0,2);
        lcd.print("                     ");
        lcd.setCursor(0,3);
        lcd.print("                     ");
      }
  

      char buffer[TAMANHO];
      serializeJson(json, buffer);
      MQTT.publish(TOPIC_PUBLISH_TEMP, buffer);
      Serial.println(buffer);
      lcd.setCursor(0, 0);
      lcd.print("Temp = "); 
      lcd.setCursor(7,0);
      lcd.print(temp);   
      delay(1000);
}
  
