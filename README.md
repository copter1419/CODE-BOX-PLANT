# 1.เตรียมอุปกรณ์ทั้งหมดที่ใช้ในโปรเจค
1. ESP8266 (NodeMCU) - สำหรับควบคุมระบบและเชื่อมต่อ Wi-Fi
2. โมดูลปั๊มน้ำขนาดเล็ก - สำหรับจ่ายน้ำไปยังต้นผัก
3. รีเลย์ 5V - สำหรับควบคุมการเปิด/ปิดปั๊มน้ำ
4. เซ็นเซอร์วัดความชื้นในดิน - วัดระดับความชื้นในดิน
5. เซ็นเซอร์วัดระดับน้ำ - ตรวจสอบระดับน้ำในถัง
6. แบตเตอรี่ (หรือแหล่งจ่ายไฟ) - สำหรับจ่ายพลังงานให้ระบบ
7. สายยางขนาดเล็ก - สำหรับจ่ายน้ำไปยังดิน
8. แผง PCB และสายไฟ - สำหรับเชื่อมต่ออุปกรณ์ต่างๆ
9. ภาชนะสำหรับใส่น้ำ - ใช้เป็นแหล่งเก็บน้ำ
10. หัวน้ำหยด (Drip Head) (ถ้าต้องการควบคุมน้ำที่จ่ายได้แม่นยำ)
11. มือถือ/คอมพิวเตอร์ - สำหรับควบคุมและแสดงค่าผ่านเว็บเซิร์ฟเวอร์

# 2.ติดตั้งโค้ดและไลเบอรี่สำหรับโปรเจค
โค้ด
```
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
```
รายชื่อlibralyที่ใช้
1. ESP8266WiFi - สำหรับเชื่อมต่อ Wi-Fi
2. ESPAsyncWebServer - สำหรับสร้างเว็บเซิร์ฟเวอร์
3. Wire - สำหรับการเชื่อมต่อเซ็นเซอร์บางตัว (ถ้าใช้ I2C)
4. DHT (ถ้าเพิ่มเซ็นเซอร์วัดอุณหภูมิและความชื้น) - สำหรับเซ็นเซอร์วัดสภาพแวดล้อม

# 3.ประกอบอุุปกรณ์ต่างๆเข้าด้วยกัน
ตัวอย่างภาพ
![download](https://github.com/user-attachments/assets/b1a48ef8-2f44-4274-8488-8b27fb9d285f)


# 4.ปลูกผักจริงแล้วติดตามการเจริญเติบโต

# 5.สรุปผลการทดลอง
