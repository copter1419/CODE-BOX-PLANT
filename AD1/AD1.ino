#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

// ตั้งค่า WiFi
const char* ssid = "ihereta";
const char* password = "sikibidi";

// พอร์ตที่ใช้ควบคุม
#define RELAY_PIN D1
#define SOIL_SENSOR_PIN A0
#define WATER_LEVEL_PIN D2

AsyncWebServer server(80);

void setup() {
  // ตั้งค่า Serial
  Serial.begin(115200);

  // เชื่อมต่อ Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // ตั้งค่า PIN
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SOIL_SENSOR_PIN, INPUT);
  pinMode(WATER_LEVEL_PIN, INPUT);

  // หน้าเว็บเซิร์ฟเวอร์
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int soilMoisture = analogRead(SOIL_SENSOR_PIN);
    int waterLevel = digitalRead(WATER_LEVEL_PIN);
    String html = "<h1>Plant Monitoring System</h1>";
    html += "<p>Soil Moisture: " + String(soilMoisture) + "</p>";
    html += "<p>Water Level: " + (waterLevel == LOW ? "Low" : "Sufficient") + "</p>";
    request->send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  // อ่านค่าความชื้นในดิน
  int soilMoisture = analogRead(SOIL_SENSOR_PIN);
  Serial.println("Soil Moisture: " + String(soilMoisture));

  // อ่านระดับน้ำ
  int waterLevel = digitalRead(WATER_LEVEL_PIN);

  // ควบคุมปั๊มน้ำ
  if (soilMoisture < 500 && waterLevel == HIGH) {
    digitalWrite(RELAY_PIN, LOW); // เปิดปั๊มน้ำ
    Serial.println("Pump ON");
  } else {
    digitalWrite(RELAY_PIN, HIGH); // ปิดปั๊มน้ำ
    Serial.println("Pump OFF");
  }

  delay(2000);
}