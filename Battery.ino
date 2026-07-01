#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ========== OLED Settings ==========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ========== Voltage Sensor ==========
#define VOLT_PIN 34
#define VOLT_DIVIDER 5.0f
#define REF_VOLTAGE 3.3f
#define ADC_RESOLUTION 4095.0f

// ========== Access Point Settings ==========
const char* ap_ssid = "BatteryMonitor";     // WiFi name
const char* ap_password = "12345678";       // Password (min 8 chars)

WebServer server(80);

// ========== Functions ==========
float readBatteryVoltage() {
  int raw = analogRead(VOLT_PIN);
  return (raw / ADC_RESOLUTION) * REF_VOLTAGE * VOLT_DIVIDER;
}

String getStatus(float voltage) {
  if (voltage >= 12.6) return "Full";
  else if (voltage >= 12.3) return "Good";
  else if (voltage >= 12.0) return "Weak";
  else if (voltage >= 11.5) return "Bad";
  else return "Replace";
}

int getPercent(float voltage) {
  if (voltage >= 12.6) return 100;
  else if (voltage >= 12.3) return 80;
  else if (voltage >= 12.0) return 60;
  else if (voltage >= 11.5) return 30;
  else return 10;
}

void updateOLED(float voltage, bool isCharging) {
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(voltage, 2);
  display.print("V");
  
  display.setTextSize(1);
  display.setCursor(0, 22);
  if (isCharging) {
    display.print("CHARGING...");
  } else {
    display.print("Status: ");
    display.print(getStatus(voltage));
  }
  
  int percent = getPercent(voltage);
  display.setCursor(0, 40);
  display.print(percent);
  display.print("%");
  
  display.drawRect(30, 38, 100, 12, SSD1306_WHITE);
  display.fillRect(30, 39, map(percent, 0, 100, 0, 100), 10, SSD1306_WHITE);
  
  // Show IP on OLED
  display.setTextSize(1);
  display.setCursor(0, 54);
  display.print(WiFi.softAPIP());
  
  display.display();
}

// ========== Web Page ==========
void handleRoot() {
  float voltage = readBatteryVoltage();
  bool isCharging = (voltage > 13.5);
  int percent = getPercent(voltage);
  String status = getStatus(voltage);
  
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Battery Monitor</title>
    <style>
      * { margin: 0; padding: 0; box-sizing: border-box; }
      body {
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        background: linear-gradient(135deg, #0a0a1a, #1a1a3e, #0d1b2a);
        color: #fff;
        min-height: 100vh;
        display: flex;
        justify-content: center;
        align-items: center;
        padding: 20px;
      }
      .container {
        background: rgba(255,255,255,0.05);
        backdrop-filter: blur(10px);
        border-radius: 30px;
        padding: 35px;
        max-width: 480px;
        width: 100%;
        box-shadow: 0 20px 60px rgba(0,0,0,0.7);
        border: 1px solid rgba(255,255,255,0.08);
      }
      h1 {
        text-align: center;
        font-size: 22px;
        color: #4fc3f7;
        letter-spacing: 3px;
        margin-bottom: 5px;
      }
      .subtitle {
        text-align: center;
        color: #666;
        font-size: 12px;
        margin-bottom: 25px;
        letter-spacing: 1px;
      }
      .voltage {
        text-align: center;
        font-size: 72px;
        font-weight: 300;
        color: #4fc3f7;
        padding: 10px 0;
        text-shadow: 0 0 40px rgba(79,195,247,0.2);
        font-family: 'Courier New', monospace;
      }
      .voltage span { font-size: 28px; color: #555; }
      
      .status-box {
        background: rgba(255,255,255,0.03);
        border-radius: 15px;
        padding: 15px;
        margin: 15px 0;
        text-align: center;
        border: 1px solid rgba(255,255,255,0.05);
      }
      .status-label { color: #888; font-size: 12px; text-transform: uppercase; letter-spacing: 2px; }
      .status-value {
        font-size: 28px;
        font-weight: bold;
        margin-top: 5px;
      }
      .charging { color: #ffd54f; }
      .full { color: #4caf50; }
      .good { color: #8bc34a; }
      .weak { color: #ff9800; }
      .bad { color: #f44336; }
      .replace { color: #d32f2f; }
      
      .bar-container {
        background: rgba(255,255,255,0.08);
        border-radius: 20px;
        height: 28px;
        margin: 15px 0;
        overflow: hidden;
        position: relative;
      }
      .bar {
        height: 100%;
        border-radius: 20px;
        transition: width 0.5s ease;
        background: linear-gradient(90deg, #4fc3f7, #00e676);
      }
      .bar-percent {
        position: absolute;
        right: 12px;
        top: 50%;
        transform: translateY(-50%);
        font-size: 13px;
        font-weight: bold;
        color: #fff;
        text-shadow: 0 0 10px rgba(0,0,0,0.8);
      }
      
      .info-grid {
        display: grid;
        grid-template-columns: 1fr 1fr;
        gap: 10px;
        margin: 15px 0;
      }
      .info-item {
        background: rgba(255,255,255,0.03);
        border-radius: 10px;
        padding: 10px;
        text-align: center;
        border: 1px solid rgba(255,255,255,0.05);
      }
      .info-item .label { font-size: 10px; color: #666; text-transform: uppercase; }
      .info-item .value { font-size: 16px; font-weight: bold; color: #4fc3f7; margin-top: 3px; }
      
      .btn-group {
        display: grid;
        grid-template-columns: 1fr 1fr;
        gap: 10px;
        margin-top: 15px;
      }
      .btn {
        padding: 14px;
        border: none;
        border-radius: 12px;
        color: #fff;
        font-size: 16px;
        font-weight: bold;
        cursor: pointer;
        transition: all 0.3s;
        text-align: center;
        text-decoration: none;
      }
      .btn-refresh {
        background: linear-gradient(135deg, #4fc3f7, #0288d1);
      }
      .btn-refresh:hover { transform: scale(1.02); opacity: 0.9; }
      .btn-ip {
        background: rgba(255,255,255,0.08);
        font-size: 12px;
        color: #888;
        cursor: default;
      }
      
      .footer {
        text-align: center;
        font-size: 10px;
        color: #444;
        margin-top: 20px;
        letter-spacing: 1px;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>🔋 BATTERY</h1>
      <div class="subtitle">ESP32 SMART MONITOR</div>
      
      <div class="voltage">
        )rawliteral";
  html += String(voltage, 2);
  html += R"rawliteral(<span> V</span>
      </div>
      
      <div class="status-box">
        <div class="status-label">Battery Status</div>
        <div class="status-value )rawliteral";
  if (isCharging) html += "charging";
  else if (status == "Full") html += "full";
  else if (status == "Good") html += "good";
  else if (status == "Weak") html += "weak";
  else if (status == "Bad") html += "bad";
  else html += "replace";
  html += R"rawliteral(">
          )rawliteral";
  if (isCharging) html += "⚡ CHARGING";
  else html += status;
  html += R"rawliteral(
        </div>
      </div>
      
      <div class="bar-container">
        <div class="bar" style="width: )rawliteral";
  html += String(percent);
  html += R"rawliteral(%"></div>
        <div class="bar-percent">)rawliteral";
  html += String(percent);
  html += R"rawliteral(%</div>
      </div>
      
      <div class="info-grid">
        <div class="info-item">
          <div class="label">IP Address</div>
          <div class="value">192.168.4.1</div>
        </div>
        <div class="info-item">
          <div class="label">WiFi</div>
          <div class="value" style="font-size:12px;">BatteryMonitor</div>
        </div>
      </div>
      
      <div class="btn-group">
        <button class="btn btn-refresh" onclick="location.reload()">🔄 Refresh</button>
        <div class="btn btn-ip">⚡ AP Mode</div>
      </div>
      
      <div class="footer">Auto-refresh every 3s • Connect to "BatteryMonitor" WiFi</div>
    </div>
    
    <script>
      setTimeout(function() { location.reload(); }, 3000);
    </script>
  </body>
  </html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

// ========== Setup ==========
void setup() {
  Serial.begin(115200);
  
  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED error");
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Starting AP...");
  display.display();
  
  // Setup Access Point
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access Point Started");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("Password: ");
  Serial.println(ap_password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  
  // Web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  
  // Show IP on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("AP Mode");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println("192.168.4.1");
  display.display();
}

// ========== Loop ==========
void loop() {
  server.handleClient();
  
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();
    float voltage = readBatteryVoltage();
    bool isCharging = (voltage > 13.5);
    updateOLED(voltage, isCharging);
  }
}