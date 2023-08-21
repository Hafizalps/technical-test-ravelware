#include <WiFi.h>
#include <PubSubClient.h>
// Modbus
/*#include <ModbusMaster.h>
#include <HardwareSerial.h>
*/
#include <ArduinoJson.h>
#include "global.h"

WiFiClient espClient;
PubSubClient client(espClient);

// Modbus
/*
// Inisialisasi objek ModbusMaster dengan Serial1 (TX1, RX1) untuk modul 1 power meter
ModbusMaster nodeModul1;

// Inisialisasi objek ModbusMaster dengan Serial2 (TX2, RX2) untuk modul 2 temp sensor
ModbusMaster nodeModul2;
*/

void setup() {
  Serial.begin(115200);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW); // Matikan SSR saat memulai
  WiFi.begin(ssid, password);

  // Wi-Fi & MQTT
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT Broker");
    } else {
      Serial.print("MQTT Connection Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }

  /*
  // Modbus Serial
  // Inisialisasi komunikasi Serial1 (TX1, RX1) untuk modul 1
  Serial1.begin(9600, SERIAL_8N1, 3, 1); // RX1 (GPIO3), TX1 (GPIO1)
  Serial1.setTimeout(200);

  // Inisialisasi komunikasi Serial2 (TX2, RX2) untuk modul 2
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX2 (GPIO16), TX2 (GPIO17)
  Serial2.setTimeout(200);

  // Inisialisasi objek ModbusMaster untuk modul 1 dengan alamat slave 1
  nodeModul1.begin(1, Serial1);

  // Inisialisasi objek ModbusMaster untuk modul 2 dengan alamat slave 2
  nodeModul2.begin(2, Serial2);
  */
}

void loop() {
  // Modbus
  /*
  uint8_t result, result2;
  uint16_t data_node[2], data_node2;
  int i;
  float reading;
  */

  // Modbus
  /*
  for(i=0; i<=2; i++){
    result = nodeModul1.readHoldingRegisters(data_register[i], 1);
    if (result == nodeModul1.ku8MBSuccess) {
      Serial.println("Success, Received data: ");
      data_node[0]=nodeModul1.getResponseBuffer(0x00);
      data_node[1]=nodeModul1.getResponseBuffer(0x01);

      // Baca voltage dari modul 1
      if(data_register[i] == 0x0123){
        Serial.print("Volatge: ");
        reading = *((float *)data_node);
        Serial.print(reading);
        Serial.println(" Volt");
      }
      // Baca current dari modul 1
      if(data_register[i] == 0x0234){
        Serial.print("Current: ");
        reading = *((float *)data_node);
        Serial.print(reading);
        Serial.println(" Amps");
      }
      // Baca power dari modul 1
      if(data_register[i] == 0x0345){
        Serial.print("Power: ");
        reading = *((float *)data_node);
        Serial.print(reading);
        Serial.println(" Watt");
      }
    } else {
      Serial.print("Gagal membaca modul 1");
      Serial.print(result, HEX);
      Serial.println("");
    }
  }

  // Baca register holding dari modul 2
  result2 = nodeModul2.readHoldingRegisters(0x03E8, 1); // Alamat register dan jumlah register

  // Cek hasil operasi modul 2
  if (result2 == nodeModul2.ku8MBSuccess) {
    data_node2 = nodeModul2.getResponseBuffer(0); // Data dari buffer respons
    Serial.print("Data dari modul 2: ");
    Serial.print(data_node2);
    Serial.println(" C");
  } else {
    Serial.print("Gagal membaca modul 2: ");
    Serial.println(result2, HEX);
  }
  */
  
  // Simulasi pengiriman data (Data dummy)
  float voltage = random(190, 221); // Contoh voltage acak antara 190-220 V
  float current = random(0, 7); // Contoh current acak antara 0-6 A
  float power = voltage * current;
  float currentTemperature = random(20, 30); // Contoh suhu acak antara 20-30 derajat Celsius

  // Cek apakah suhu mengalami kenaikan 2%
  if ((currentTemperature - lastTemperature) / lastTemperature >= TEMPERATURE_THRESHOLD) {
    digitalWrite(FAN_PIN, HIGH); // Nyalakan FAN
    fanStatus = "ON";
  } else {
    digitalWrite(FAN_PIN, LOW);
    fanStatus = "OFF";
  }
  lastTemperature = currentTemperature;

  // Konversi ke tipe data string
  String voltagePayload = String(voltage);
  String currentPayload = String(current);
  String powerPayload = String(power);
  String tempPayload = String(lastTemperature);
  
  // Membuat objek JSON
  StaticJsonDocument<200> jsonDocument;
  jsonDocument["status"] = "OK";
  jsonDocument["deviceID"] = "hafizal-ps";
  JsonObject data = jsonDocument.createNestedObject("data");
  data["v"] = voltagePayload;
  data["i"] = currentPayload;
  data["pa"] = powerPayload;
  data["temp"] = tempPayload;
  data["fan"] = fanStatus;
  

  // Mengonversi objek JSON menjadi String
  String payload;
  serializeJson(jsonDocument, payload);

  client.publish("DATA/LOCAL/SENSOR/PANEL_1", payload.c_str());
  Serial.println(payload);
    
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle pesan yang diterima jika diperlukan
}
