const char* ssid = "ACUPUNCTURE CLINIC 1";
const char* password = "0351496477";
const char* mqttServer = "192.168.1.7";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

// voltage, current and power data register of Selec EM4M
uint16_t data_register[3] = {0x0123, 0x0234, 0x0345};

float lastTemperature = 0.0; // Nilai suhu sebelumnya
const float TEMPERATURE_THRESHOLD = 0.02; // Kenaikan suhu sebesar 2%

const int FAN_PIN = 32; // Pin GPIO32 untuk mengendalikan SSR (FAN)

String fanStatus;
