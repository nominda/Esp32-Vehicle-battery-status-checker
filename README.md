# 🔋 ESP32 Smart Battery Monitor

Monitor your battery status from anywhere via WiFi!  
This project uses an ESP32, OLED Display, and Voltage Sensor to display real-time battery voltage, percentage, and status (Good/Weak/Bad). **No router needed** - it creates its own WiFi Access Point.

---

## ✨ Features

- ✅ **Real-time voltage monitoring** (12V Lead-Acid / Li-ion)
- ✅ **Smart Status Detection** - Full / Good / Weak / Bad / Replace
- ✅ **Charging Detection** - Shows "CHARGING" when voltage > 13.5V ⚡
- ✅ **Graphical OLED Display** - 128x64 with percentage bar
- ✅ **WiFi Access Point Mode** - Connect directly from your Phone/Laptop
- ✅ **Mobile-Friendly Web UI** - Access from any browser
- ✅ **Auto-Refresh** - Updates every 3 seconds automatically
- ✅ **JSON API** - `/data` endpoint for custom integrations
- ✅ **Up to 8 devices** can connect simultaneously

---

## 🛠️ Hardware Required

| Component | Quantity |
|-----------|----------|
| ESP32 Development Board | 1 |
| 128x64 I2C OLED Display | 1 |
| 0-25V DC Voltage Sensor Module | 1 |
| Female-to-Female Jumper Wires | 4-5 |
| 12V Battery (for testing) | 1 |

---

## 🔌 Wiring Diagram

### Voltage Sensor → ESP32
| Sensor Pin | ESP32 Pin |
|------------|-----------|
| VCC | Battery (+) Positive Terminal |
| GND | ESP32 GND (and Battery -) |
| SIG | GPIO 34 (ADC) |

### OLED Display → ESP32 (I2C)
| OLED Pin | ESP32 Pin |
|----------|-----------|
| VCC | 3.3V |
| GND | GND |
| SCL | GPIO 22 |
| SDA | GPIO 21 |


---

## 📂 Software Setup

### 1. Install Arduino IDE Libraries
From Library Manager, install:
- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `Adafruit BusIO`

### 2. Upload the Code
- Clone or Download this repository.
- Open `ESP32_Battery_Monitor.ino` in Arduino IDE.
- Select **ESP32 Dev Module** as the board.
- Click **Upload**.

---

## 📱 How to Use

### Step 1: Power on the ESP32
The OLED display will show the IP address: **`192.168.4.1`**

### Step 2: Connect to WiFi
Go to WiFi Settings on your Phone/Laptop:
- **SSID:** `BatteryMonitor`
- **Password:** `12345678`

### Step 3: Open the Web Interface
Open your browser and type **`192.168.4.1`** in the address bar.

### Step 4: Monitor Your Battery
You will see:
- Voltage (V)
- Status (Full/Good/Weak/Bad/Replace)
- Percentage Bar
- Charging Status (if voltage > 13.5V)

---

## 📊 JSON API

In addition to the Web UI, you can get raw data in JSON format:

```
http://192.168.4.1/data
```

**Response Example:**
```json
{
  "voltage": 12.45,
  "percent": 80,
  "status": "Good",
  "charging": false
}
```

---

## 🔧 Troubleshooting

| Issue | Solution |
|-------|----------|
| OLED not working | Verify I2C address is `0x3C` or `0x3D` |
| Can't connect to WiFi | Make sure password is `12345678` |
| Wrong voltage reading | Adjust `VOLT_DIVIDER` value (5.0 for 0-25V module) |
| Nothing on Serial Monitor | Set baud rate to `115200` |

---

## 📝 Customization

### Change WiFi Credentials
```cpp
const char* ap_ssid = "YourSSID";
const char* ap_password = "YourPassword"; // Must be at least 8 characters
```

### Adjust Voltage Divider
Change `VOLT_DIVIDER` according to your sensor module:
```cpp
#define VOLT_DIVIDER 5.0f   // 0-25V module
// #define VOLT_DIVIDER 11.0f  // 10k + 1k divider
// #define VOLT_DIVIDER 2.0f   // 0-5V module
```

---

## 🎥 Demo Video

[![Video] [https://www.linkedin.com/posts/nominda-yasasvin-793466416_esp32-iot-batterymonitor-ugcPost-7477910595640483840-9K0H/?utm_source=share&utm_medium=member_android&rcm=ACoAAGnJYt8BJicbbasMIYkevcdRVxzcBP5voLQ]

> *The video shows ESP32 booting, OLED display, connecting via Phone WiFi, and the Web UI in action.*

---

## 🤝 Contributing

Have an idea to improve this project?
1. Fork the repository
2. Create your branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📄 License

This project is distributed under the **MIT License**. See the `LICENSE` file for more details.

---

## 📬 Connect with Me

- GitHub: [Nominda] [https://github.com/nominda]
- LinkedIn: [Nominda Yasasvin]   [https://www.linkedin.com/in/nominda-yasasvin]

---

**⭐ If you found this useful, please give it a Star!**

---

**Built with ❤️ by Nominda Yasasvin ** | Keep your batteries safe! 🔋
