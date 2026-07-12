/*
  =============================================================================
  PROJECT EXO (Formerly Project Skyfall) - MASTER FIRMWARE
  Target Hardware: ESP32-S3 Zero
  =============================================================================
*/

#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

// --- Hardware Settings ---
static const uint8_t OLED_ADDR   = 0x3C;
static const int SCREEN_WIDTH    = 128;
static const int SCREEN_HEIGHT   = 64;
static const uint8_t SDA_PIN     = 8;
static const uint8_t SCL_PIN     = 9;
static const uint8_t BUTTON_PIN  = 10;
static const uint8_t BUZZER_PIN  = 6;
static const uint8_t UART1_TX    = 4;
static const uint8_t UART1_RX    = 5;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- I2C Addresses ---
static const uint8_t EEPROM24C32_ADDR = 0x50;
static const uint8_t BMP280_ADDR      = 0x76;
static const uint8_t BMP180_ADDR      = 0x77;
static const uint8_t MPU6050_ADDR     = 0x68;

// --- Version info ---
static const char* FW_VERSION = "00x00-EXO";
static const char* FW_AUTHOR  = "Melih uzun";

// --- Atlas Logo Bitmap (128x64px) ---
const unsigned char epd_bitmap_images [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// --- Data Structures & State Machine ---
struct __attribute__((packed)) ConfigData {
  uint8_t magic;
  uint8_t exoType;        // 0=A.S.T., 1=Specialist, 2=Assault
  uint8_t brightness;     // 0-255
  uint8_t bootAnim;       // 0=Off, 1=On
  uint8_t bootSound;      // 0=Off, 1=On
  uint8_t volume;         // 0=Mute, 1=Max
  uint16_t postDelay;     // Delay between items in ms
  uint32_t bootCount;
  uint8_t checksum;
};

ConfigData cfg;
bool eepromAvailable = false;

enum ScreenState : uint8_t {
  SCREEN_BOOT = 0,
  SCREEN_EXO_SELECT,
  SCREEN_MAIN_MENU,
  SCREEN_STATUS,
  SCREEN_SENSORS,
  SCREEN_CAMERA_MENU,
  SCREEN_RECORDING,
  SCREEN_SETTINGS,
  SCREEN_DEVELOPER_MENU,
  SCREEN_UART_MONITOR,
  SCREEN_I2C_SCAN,
  SCREEN_SENSOR_RAW,
  SCREEN_MEMORY_INFO,
  SCREEN_ABOUT
};

ScreenState screen = SCREEN_BOOT;

// --- Menu Infrastructure ---
const char* EXO_NAMES[] = { "A.S.T.", "Specialist", "Assault" };
const char* MAIN_MENU[] = { "Status", "Sensors", "Camera", "Settings", "Exosuit", "About" };
const char* CAMERA_MENU[] = { "Photo", "Record", "Status", "Settings", "Back" };
const char* SETTINGS_MENU[] = { "Brightness", "Boot Anim", "Boot Sound", "Volume", "POST Delay", "Factory Reset", "Developer Opt", "Back" };
const char* DEV_MENU[] = { "UART Monitor", "I2C Scan", "Sensor Raw", "Memory Info", "Back" };

static const uint8_t EXO_COUNT      = 3;
static const uint8_t MAIN_COUNT     = 6;
static const uint8_t CAMERA_COUNT   = 5;
static const uint8_t SETTINGS_COUNT = 8;
static const uint8_t DEV_COUNT      = 5;

uint8_t exoCursor = 0;
uint8_t mainCursor = 0;
uint8_t cameraCursor = 0;
uint8_t settingsCursor = 0;
uint8_t devCursor = 0;

uint8_t statusPage = 0;
uint8_t sensorPage = 0;
uint8_t scanPage = 0;

// --- Camera Communication State ---
char camStatusStr[32] = "OFFLINE";
float camFPS = 0.0;
char camFreeSpace[16] = "0MB";
bool isRecordingActive = false;
uint32_t recordStartMs = 0;

// --- Button Engine Variables ---
enum ButtonEvent : uint8_t { EVT_NONE = 0, EVT_SHORT, EVT_LONG };
static bool btnStable = HIGH;
static bool btnLastRaw = HIGH;
static uint32_t btnLastEdge = 0;
static uint32_t btnDownSince = 0;
static bool btnLongSent = false;
static const uint16_t DEBOUNCE_MS = 25;
static const uint16_t LONG_PRESS_MS = 700;
static const uint16_t SHORT_PRESS_MIN_MS = 40;

// --- Hardware Presence Matrix ---
bool i2cOLEDPresent = false;
bool i2cEEPROMPresent = false;
bool i2cBMP280Present = false;
bool i2cBMP180Present = false;
bool i2cMPU6050Present = false;
bool camUartPresent = false;

uint8_t scanAddrs[24];
uint8_t scanCount = 0;

// --- Sensor Data Pipeline Cache ---
struct SensorPageData { bool ok = false; float tempC = NAN; float pressureHpa = NAN; float altitudeM = NAN; };
struct MPUData { bool ok = false; float tempC = NAN; float pitch = NAN; float roll = NAN; float ax = 0, ay = 0, az = 0; float gx = 0, gy = 0, gz = 0; };

SensorPageData bmp180Data;
SensorPageData bmp280Data;
MPUData mpuData;
uint32_t lastSensorReadMs = 0;

// Calibration Register Structs
struct BMP180Calib { int16_t ac1, ac2, ac3; uint16_t ac4, ac5, ac6; int16_t b1, b2, mb, mc, md; bool ok = false; } bmp180Cal;
struct BMP280Calib { uint16_t dig_T1; int16_t dig_T2, dig_T3; uint16_t dig_P1; int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9; bool ok = false; int32_t t_fine; } bmp280Cal;

// --- Forward Declarations ---
void sendCamCmd(const char* cmd);
bool waitForCamResponse(char* buffer, size_t maxLen, uint16_t timeoutMs);
void syncCamStatus();
void buzzerTone(uint16_t freq);
void buzzerOff();
void beep(uint16_t freq, uint16_t ms);
void playPostSound(bool success);

// =============================================================================
// BUZZER DRIVER VIA LEDC
// =============================================================================
void buzzerInit() {
  ledcAttach(BUZZER_PIN, 2000, 8);
  buzzerOff();
}

void buzzerTone(uint16_t freq) {
  if (cfg.volume == 0) { ledcWriteTone(BUZZER_PIN, 0); return; }
  ledcWriteTone(BUZZER_PIN, freq);
}

void buzzerOff() {
  ledcWriteTone(BUZZER_PIN, 0);
}

void beep(uint16_t freq, uint16_t ms) {
  if (freq == 0 || ms == 0) return;
  buzzerTone(freq);
  delay(ms);
  buzzerOff();
}

void playIndustrialPostSound() {
  if (!cfg.bootSound) return;
  for (int i = 0; i < 3; i++) {
    ledcWriteTone(BUZZER_PIN, 120);
    delay(15);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(280);
  }
  ledcWriteTone(BUZZER_PIN, 60);
  delay(120);
  buzzerOff();
}

void playPostSound(bool success) {
  if (success) {
    beep(2400, 40);
  } else {
    beep(400, 250);
  }
}

// =============================================================================
// EEPROM 24C32 RAW UTILITIES
// =============================================================================
uint8_t cfgChecksum(const ConfigData& c) {
  const uint8_t* p = (const uint8_t*)&c;
  uint8_t sum = 0;
  for (size_t i = 0; i < sizeof(ConfigData) - 1; i++) sum ^= p[i];
  return (uint8_t)(sum ^ 0x5A);
}

bool cfgValid(const ConfigData& c) {
  return (c.magic == 0xA7 && c.exoType < EXO_COUNT && c.checksum == cfgChecksum(c));
}

void eepromWaitReady() {
  if (!eepromAvailable) return;
  for (uint8_t i = 0; i < 60; i++) {
    Wire.beginTransmission(EEPROM24C32_ADDR);
    if (Wire.endTransmission() == 0) return;
    delay(2);
  }
}

uint8_t eepromReadByte(uint16_t memAddr) {
  if (!eepromAvailable) return 0xFF;
  Wire.beginTransmission(EEPROM24C32_ADDR);
  Wire.write((uint8_t)(memAddr >> 8));
  Wire.write((uint8_t)(memAddr & 0xFF));
  if (Wire.endTransmission(false) != 0) return 0xFF;
  if (Wire.requestFrom((uint8_t)EEPROM24C32_ADDR, (uint8_t)1) != 1) return 0xFF;
  return Wire.read();
}

void eepromWriteByte(uint16_t memAddr, uint8_t value) {
  if (!eepromAvailable) return;
  Wire.beginTransmission(EEPROM24C32_ADDR);
  Wire.write((uint8_t)(memAddr >> 8));
  Wire.write((uint8_t)(memAddr & 0xFF));
  Wire.write(value);
  Wire.endTransmission();
  eepromWaitReady();
}

void loadConfig() {
  if (eepromAvailable) {
    uint8_t* ptr = (uint8_t*)&cfg;
    for (size_t i = 0; i < sizeof(cfg); i++) {
      ptr[i] = eepromReadByte(0x0000 + i);
    }
  }
  if (!cfgValid(cfg)) {
    cfg.magic = 0xA7;
    cfg.exoType = 0;
    cfg.brightness = 255;
    cfg.bootAnim = 1;
    cfg.bootSound = 1;
    cfg.volume = 1;
    cfg.postDelay = 400;
    cfg.bootCount = 0;
    cfg.checksum = cfgChecksum(cfg);
    saveConfig();
  }
}

void saveConfig() {
  cfg.checksum = cfgChecksum(cfg);
  if (!eepromAvailable) return;
  const uint8_t* ptr = (const uint8_t*)&cfg;
  for (size_t i = 0; i < sizeof(cfg); i++) {
    eepromWriteByte(0x0000 + i, ptr[i]);
  }
}

// =============================================================================
// I2C ENGINES & SCANNER
// =============================================================================
bool i2cPresent(uint8_t addr) {
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}

bool i2cWrite8(uint8_t addr, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool i2cRead(uint8_t addr, uint8_t reg, uint8_t* buf, size_t len) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(addr, (uint8_t)len) != len) return false;
  for (size_t i = 0; i < len; i++) buf[i] = Wire.read();
  return true;
}

uint16_t i2cReadU16BE(uint8_t addr, uint8_t reg) {
  uint8_t b[2];
  if (!i2cRead(addr, reg, b, 2)) return 0;
  return (uint16_t)((b[0] << 8) | b[1]);
}

int16_t i2cReadS16BE(uint8_t addr, uint8_t reg) { return (int16_t)i2cReadU16BE(addr, reg); }

uint32_t i2cReadU24BE(uint8_t addr, uint8_t reg) {
  uint8_t b[3];
  if (!i2cRead(addr, reg, b, 3)) return 0;
  return ((uint32_t)b[0] << 16) | ((uint32_t)b[1] << 8) | b[2];
}

void runI2CScan() {
  scanCount = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    if (i2cPresent(addr)) {
      if (scanCount < sizeof(scanAddrs)) scanAddrs[scanCount++] = addr;
    }
  }
}

void setDisplayMaxBrightness() {
  display.dim(false);
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(cfg.brightness);
}

// =============================================================================
// SENSOR PIPELINE IMPLEMENTATION
// =============================================================================
void bmp180LoadCalibration() {
  if (!i2cBMP180Present) return;
  bmp180Cal.ac1 = i2cReadS16BE(BMP180_ADDR, 0xAA);
  bmp180Cal.ac2 = i2cReadS16BE(BMP180_ADDR, 0xAC);
  bmp180Cal.ac3 = i2cReadS16BE(BMP180_ADDR, 0xAE);
  bmp180Cal.ac4 = i2cReadU16BE(BMP180_ADDR, 0xB0);
  bmp180Cal.ac5 = i2cReadU16BE(BMP180_ADDR, 0xB2);
  bmp180Cal.ac6 = i2cReadU16BE(BMP180_ADDR, 0xB4);
  bmp180Cal.b1  = i2cReadS16BE(BMP180_ADDR, 0xB6);
  bmp180Cal.b2  = i2cReadS16BE(BMP180_ADDR, 0xB8);
  bmp180Cal.mb  = i2cReadS16BE(BMP180_ADDR, 0xBA);
  bmp180Cal.mc  = i2cReadS16BE(BMP180_ADDR, 0xBC);
  bmp180Cal.md  = i2cReadS16BE(BMP180_ADDR, 0xBE);
  bmp180Cal.ok  = true;
}

void bmp280LoadCalibration() {
  if (!i2cBMP280Present) return;
  bmp280Cal.dig_T1 = i2cReadU16BE(BMP280_ADDR, 0x88);
  bmp280Cal.dig_T2 = i2cReadS16BE(BMP280_ADDR, 0x8A);
  bmp280Cal.dig_T3 = i2cReadS16BE(BMP280_ADDR, 0x8C);
  bmp280Cal.dig_P1 = i2cReadU16BE(BMP280_ADDR, 0x8E);
  bmp280Cal.dig_P2 = i2cReadS16BE(BMP280_ADDR, 0x90);
  bmp280Cal.dig_P3 = i2cReadS16BE(BMP280_ADDR, 0x92);
  bmp280Cal.dig_P4 = i2cReadS16BE(BMP280_ADDR, 0x94);
  bmp280Cal.dig_P5 = i2cReadS16BE(BMP280_ADDR, 0x96);
  bmp280Cal.dig_P6 = i2cReadS16BE(BMP280_ADDR, 0x98);
  bmp280Cal.dig_P7 = i2cReadS16BE(BMP280_ADDR, 0x9A);
  bmp280Cal.dig_P8 = i2cReadS16BE(BMP280_ADDR, 0x9C);
  bmp280Cal.dig_P9 = i2cReadS16BE(BMP280_ADDR, 0x9E);
  bmp280Cal.ok = true;
  i2cWrite8(BMP280_ADDR, 0xF5, 0xA0);
  i2cWrite8(BMP280_ADDR, 0xF4, 0x27);
}

void mpu6050Init() {
  if (!i2cMPU6050Present) return;
  i2cWrite8(MPU6050_ADDR, 0x6B, 0x00);
  i2cWrite8(MPU6050_ADDR, 0x1B, 0x00);
  i2cWrite8(MPU6050_ADDR, 0x1C, 0x00);
  i2cWrite8(MPU6050_ADDR, 0x1A, 0x03);
}

bool bmp180Read(float& tempC, float& pressureHpa, float& altitudeM) {
  if (!bmp180Cal.ok) return false;
  if (!i2cWrite8(BMP180_ADDR, 0xF4, 0x2E)) return false;
  delay(5);
  int32_t UT = (int32_t)i2cReadU16BE(BMP180_ADDR, 0xF6);
  if (!i2cWrite8(BMP180_ADDR, 0xF4, 0x34)) return false;
  delay(5);
  uint32_t UP = i2cReadU24BE(BMP180_ADDR, 0xF6) >> 8;
  
  int32_t X1 = ((UT - (int32_t)bmp180Cal.ac6) * (int32_t)bmp180Cal.ac5) >> 15;
  int32_t X2 = ((int32_t)bmp180Cal.mc << 11) / (X1 + (int32_t)bmp180Cal.md);
  int32_t B5 = X1 + X2;
  tempC = ((B5 + 8) >> 4) / 10.0f;

  int32_t B6 = B5 - 4000;
  X1 = ((int32_t)bmp180Cal.b2 * ((B6 * B6) >> 12)) >> 11;
  X2 = ((int32_t)bmp180Cal.ac2 * B6) >> 11;
  int32_t X3 = X1 + X2;
  int32_t B3 = ((((int32_t)bmp180Cal.ac1 * 4 + X3) << 0) + 2) >> 2;
  X1 = ((int32_t)bmp180Cal.ac3 * B6) >> 13;
  X2 = ((int32_t)bmp180Cal.b1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  uint32_t B4 = ((uint32_t)bmp180Cal.ac4 * (uint32_t)(X3 + 32768)) >> 15;
  if (B4 == 0) return false;
  uint32_t B7 = (uint32_t)((uint32_t)UP - B3) * 50000UL;
  int32_t p = (B7 < 0x80000000UL) ? (int32_t)((B7 * 2) / B4) : (int32_t)((B7 / B4) * 2);
  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;
  p = p + ((X1 + X2 + 3791) >> 4);
  pressureHpa = p / 100.0f;
  altitudeM = 44330.0f * (1.0f - powf((pressureHpa) / 1013.25f, 0.1903f));
  return true;
}

bool bmp280Read(float& tempC, float& pressureHpa, float& altitudeM) {
  if (!bmp280Cal.ok) return false;
  uint8_t d[6];
  if (!i2cRead(BMP280_ADDR, 0xF7, d, 6)) return false;
  int32_t adc_P = ((int32_t)d[0] << 12) | ((int32_t)d[1] << 4) | (d[2] >> 4);
  int32_t adc_T = ((int32_t)d[3] << 12) | ((int32_t)d[4] << 4) | (d[5] >> 4);

  int32_t var1 = ((((adc_T >> 3) - ((int32_t)bmp280Cal.dig_T1 << 1))) * ((int32_t)bmp280Cal.dig_T2)) >> 11;
  int32_t var2 = (((((adc_T >> 4) - ((int32_t)bmp280Cal.dig_T1)) * ((adc_T >> 4) - ((int32_t)bmp280Cal.dig_T1))) >> 12) * ((int32_t)bmp280Cal.dig_T3)) >> 14;
  bmp280Cal.t_fine = var1 + var2;
  tempC = ((bmp280Cal.t_fine * 5 + 128) >> 8) / 100.0f;

  int64_t v1 = (int64_t)bmp280Cal.t_fine - 128000;
  int64_t v2 = v1 * v1 * (int64_t)bmp280Cal.dig_P6;
  v2 = v2 + ((v1 * (int64_t)bmp280Cal.dig_P5) << 17);
  v2 = v2 + (((int64_t)bmp280Cal.dig_P4) << 35);
  v1 = ((v1 * v1 * (int64_t)bmp280Cal.dig_P3) >> 8) + ((v1 * (int64_t)bmp280Cal.dig_P2) << 12);
  v1 = ((((int64_t)1 << 47) + v1) * (int64_t)bmp280Cal.dig_P1) >> 33;
  if (v1 == 0) return false;
  int64_t p = 1048576 - adc_P;
  p = (((p << 31) - v2) * 3125) / v1;
  v1 = (((int64_t)bmp280Cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  v2 = (((int64_t)bmp280Cal.dig_P8) * p) >> 19;
  p = ((p + v1 + v2) >> 8) + (((int64_t)bmp280Cal.dig_P7) << 4);
  pressureHpa = (float)p / 25600.0f;
  altitudeM = 44330.0f * (1.0f - powf((pressureHpa) / 1013.25f, 0.1903f));
  return true;
}

bool mpu6050Read(MPUData& out) {
  if (!i2cMPU6050Present) return false;
  uint8_t b[14];
  if (!i2cRead(MPU6050_ADDR, 0x3B, b, 14)) return false;
  int16_t axRaw = (int16_t)((b[0] << 8) | b[1]);
  int16_t ayRaw = (int16_t)((b[2] << 8) | b[3]);
  int16_t azRaw = (int16_t)((b[4] << 8) | b[5]);
  int16_t tRaw  = (int16_t)((b[6] << 8) | b[7]);
  int16_t gxRaw = (int16_t)((b[8] << 8) | b[9]);
  int16_t gyRaw = (int16_t)((b[10] << 8) | b[11]);
  int16_t gzRaw = (int16_t)((b[12] << 8) | b[13]);

  out.ax = axRaw / 16384.0f; out.ay = ayRaw / 16384.0f; out.az = azRaw / 16384.0f;
  out.gx = gxRaw / 131.0f;   out.gy = gyRaw / 131.0f;   out.gz = gzRaw / 131.0f;
  out.tempC = (tRaw / 340.0f) + 36.53f;
  out.pitch = atan2f(-out.ax, sqrtf(out.ay * out.ay + out.az * out.az)) * 57.29577f;
  out.roll  = atan2f(out.ay, out.az) * 57.29577f;
  out.ok = true;
  return true;
}

void readAllSensors() {
  if (millis() - lastSensorReadMs < 200) return;
  lastSensorReadMs = millis();
  if (i2cBMP180Present) bmp180Read(bmp180Data.tempC, bmp180Data.pressureHpa, bmp180Data.altitudeM);
  if (i2cBMP280Present) bmp280Read(bmp280Data.tempC, bmp280Data.pressureHpa, bmp280Data.altitudeM);
  if (i2cMPU6050Present) mpu6050Read(mpuData);
}

// =============================================================================
// UART INTER-CHIP PROTOCOL IMPLEMENTATION
// =============================================================================
void sendCamCmd(const char* cmd) {
  Serial1.print(cmd);
  Serial1.print("\n");
}

bool waitForCamResponse(char* buffer, size_t maxLen, uint16_t timeoutMs) {
  uint32_t start = millis();
  size_t idx = 0;
  while (millis() - start < timeoutMs) {
    while (Serial1.available()) {
      char c = Serial1.read();
      if (c == '\n' || c == '\r') {
        if (idx > 0) { buffer[idx] = '\0'; return true; }
      } else if (idx < maxLen - 1) {
        buffer[idx++] = c;
      }
    }
    yield();
  }
  return false;
}

void syncCamStatus() {
  sendCamCmd("STATUS");
  char buf[48];
  if (waitForCamResponse(buf, sizeof(buf), 200)) {
    camUartPresent = true;
    if (strncmp(buf, "READY", 5) == 0) {
      strcpy(camStatusStr, "READY");
      isRecordingActive = false;
    } else if (strncmp(buf, "BUSY_REC", 8) == 0) {
      strcpy(camStatusStr, "RECORDING");
      isRecordingActive = true;
      char* token = strchr(buf, '|');
      if (token) camFPS = atof(token + 1);
      token = strchr(token + 1, '|');
      if (token) strcpy(camFreeSpace, token + 1);
    } else if (strncmp(buf, "BUSY", 4) == 0) {
      strcpy(camStatusStr, "BUSY");
    }
  } else {
    strcpy(camStatusStr, "OFFLINE");
    camUartPresent = false;
  }
}

// =============================================================================
// REFACTORED UI RENDERING ENGINE (FLICKER FREE)
// =============================================================================
void drawHeader(const char* title) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(title);
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);
}

void drawFooter(const char* txt) {
  display.drawLine(0, 54, 127, 54, SSD1306_WHITE);
  display.setCursor(0, 56);
  display.setTextSize(1);
  display.print(txt);
}

void drawSelectionMenu(const char* title, const char* items[], uint8_t count, uint8_t cursor) {
  drawHeader(title);
  uint8_t start = (cursor >= 4) ? cursor - 3 : 0;
  for (uint8_t i = 0; i < 4 && (start + i) < count; i++) {
    uint8_t idx = start + i;
    display.setCursor(0, 12 + (i * 10));
    if (idx == cursor) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    display.print(items[idx]);
  }
  drawFooter("Short=Next Long=Enter");
  display.display();
}

void drawPOSTLine(const char* label, const char* value, uint16_t delayMs, bool status) {
  display.print(label);
  for (int i = 0; i < (20 - strlen(label)); i++) display.print(".");
  delay(delayMs);
  display.println(value);
  display.display();
  playPostSound(status);
}

void runVisualPOST() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("EXO OS POST VERIFY");
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);
  display.setCursor(0, 12);
  display.display();

  drawPOSTLine("OLED", "OK", cfg.postDelay, i2cOLEDPresent);
  drawPOSTLine("EEPROM", eepromAvailable ? "OK" : "FAIL", cfg.postDelay, eepromAvailable);
  drawPOSTLine("BMP180", i2cBMP180Present ? "OK" : "FAIL", cfg.postDelay, i2cBMP180Present);
  drawPOSTLine("BMP280", i2cBMP280Present ? "OK" : "FAIL", cfg.postDelay, i2cBMP280Present);
  drawPOSTLine("MPU6050", i2cMPU6050Present ? "OK" : "FAIL", cfg.postDelay, i2cMPU6050Present);
  
  // Real check on UART Module
  sendCamCmd("PING");
  char response[16];
  camUartPresent = waitForCamResponse(response, sizeof(response), 300) && (strcmp(response, "READY") == 0);
  
  drawPOSTLine("ESP32-CAM", camUartPresent ? "OK" : "FAIL", cfg.postDelay, camUartPresent);
  drawPOSTLine("UART", "OK", cfg.postDelay, true);
  drawPOSTLine("WiFi", "OFF", cfg.postDelay, true);
  drawPOSTLine("Bluetooth", "OFF", cfg.postDelay, true);
  
  delay(300);
  display.println("\nSYSTEM READY");
  display.display();
  beep(1900, 150);
  delay(800);
}

// =============================================================================
// SUB-MENU SCREEN DISPATCHERS
// =============================================================================
void renderStatusScreen() {
  drawHeader("SYSTEM STATUS");
  if (statusPage == 0) {
    display.setCursor(0, 12);
    display.printf("CPU Freq: %d MHz\n", getCpuFrequencyMhz());
    display.printf("Heap Free: %d KB\n", ESP.getFreeHeap() / 1024);
    display.printf("Int Temp: %.1f C\n", temperatureRead());
    display.printf("Uptime: %lu s\n", millis() / 1000);
  } else {
    display.setCursor(0, 12);
    display.printf("Flash Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
    display.printf("Min Free RAM: %d KB\n", ESP.getMinFreeHeap() / 1024);
    display.printf("Active EXO: %s\n", EXO_NAMES[cfg.exoType]);
    display.printf("Boot Cycles: %u\n", cfg.bootCount);
  }
  drawFooter("Short=Page Long=Back");
  display.display();
}

void renderSensorsScreen() {
  drawHeader("SENSOR DATA ENGINE");
  readAllSensors();
  display.setCursor(0, 12);
  switch (sensorPage) {
    case 0:
      display.println("[BMP180]");
      if (i2cBMP180Present) display.printf("T: %.1f C\nP: %.1f hPa\nA: %.1f m\n", bmp180Data.tempC, bmp180Data.pressureHpa, bmp180Data.altitudeM);
      else display.println("SENSOR OFFLINE");
      break;
    case 1:
      display.println("[BMP280]");
      if (i2cBMP280Present) display.printf("T: %.1f C\nP: %.1f hPa\nA: %.1f m\n", bmp280Data.tempC, bmp280Data.pressureHpa, bmp280Data.altitudeM);
      else display.println("SENSOR OFFLINE");
      break;
    case 2:
      display.println("[MPU6050 IMU]");
      if (i2cMPU6050Present) display.printf("Pitch: %.1f deg\nRoll:  %.1f deg\nT_Raw: %.1f C\n", mpuData.pitch, mpuData.roll, mpuData.tempC);
      else display.println("SENSOR OFFLINE");
      break;
    case 3:
      display.println("[INTERNAL EXOSUIT]");
      display.printf("Core Temp: %.1f C\n", temperatureRead());
      display.printf("I2C Bus: 400KHz\n");
      break;
  }
  drawFooter("Short=Page Long=Back");
  display.display();
}

void renderRecordingScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  
  // Blinking dot indicator
  if ((millis() / 400) % 2 == 0) {
    display.fillRect(6, 6, 6, 6, SSD1306_WHITE);
  }
  display.setCursor(16, 5);
  display.setTextSize(1);
  display.print("REC ACTIVE");

  uint32_t duration = (millis() - recordStartMs) / 1000;
  uint8_t mins = duration / 60;
  uint8_t secs = duration % 60;
  
  display.setCursor(10, 20);
  display.setTextSize(2);
  display.printf("%02u:%02u", mins, secs);

  display.setTextSize(1);
  display.setCursor(10, 40);
  display.printf("FPS: %.1f", camFPS);
  display.setCursor(70, 40);
  display.printf("Storage:%s", camFreeSpace);

  display.setCursor(4, 54);
  display.print("HOLD BUTTON TO STOP");
  display.display();
}

void renderI2CScanScreen() {
  drawHeader("I2C BUS SCAN");
  if (scanCount == 0) {
    display.setCursor(0, 20);
    display.print("NO DEVICES DETECTED");
  } else {
    uint8_t perPage = 4;
    uint8_t totalPages = (scanCount + perPage - 1) / perPage;
    uint8_t start = scanPage * perPage;
    uint8_t end = min((int)start + perPage, (int)scanCount);
    display.setCursor(0, 12);
    for (uint8_t i = start; i < end; i++) {
      display.printf("Addr: 0x%02X\n", scanAddrs[i]);
    }
    display.setCursor(100, 44);
    display.printf("%u/%u", scanPage + 1, totalPages);
  }
  drawFooter("Short=Page Long=Back");
  display.display();
}

void renderSensorRawScreen() {
  drawHeader("RAW TRANSDUCER BUFF");
  readAllSensors();
  display.setCursor(0, 12);
  display.printf("A: %.2f %.2f %.2f\n", mpuData.ax, mpuData.ay, mpuData.az);
  display.printf("G: %.1f %.1f %.1f\n", mpuData.gx, mpuData.gy, mpuData.gz);
  display.printf("P180: %.1f | P280: %.1f\n", bmp180Data.pressureHpa, bmp280Data.pressureHpa);
  drawFooter("Short=Refresh Long=Back");
  display.display();
}

void renderMemoryInfoScreen() {
  drawHeader("FIRMWARE MEM MAP");
  display.setCursor(0, 12);
  display.printf("Total Heap: %d B\n", ESP.getHeapSize());
  display.printf("Free Heap:  %d B\n", ESP.getFreeHeap());
  display.printf("Min Free:   %d B\n", ESP.getMinFreeHeap());
  display.printf("Max Alloc:  %d B\n", ESP.getMaxAllocHeap());
  drawFooter("Short=Refresh Long=Back");
  display.display();
}

void renderAboutScreen() {
  drawHeader("PROJECT CODENAME");
  display.setCursor(0, 12);
  display.setTextSize(2);
  display.println("EXO PROJECT");
  display.setTextSize(1);
  display.printf("FW Version: %s\n", FW_VERSION);
  display.printf("Lead Dev:   %s\n", FW_AUTHOR);
  drawFooter("Any press returns to menu");
  display.display();
}

// =============================================================================
// INPUT ENGINE STATE MACHINE & ROUTING
// =============================================================================
ButtonEvent pollButtonEvent() {
  bool raw = digitalRead(BUTTON_PIN);
  uint32_t now = millis();
  if (raw != btnLastRaw) { btnLastRaw = raw; btnLastEdge = now; }
  if ((now - btnLastEdge) < DEBOUNCE_MS) return EVT_NONE;
  if (raw != btnStable) {
    btnStable = raw;
    if (btnStable == LOW) { btnDownSince = now; btnLongSent = false; }
    else {
      uint32_t held = now - btnDownSince;
      if (!btnLongSent && held >= SHORT_PRESS_MIN_MS && held < LONG_PRESS_MS) return EVT_SHORT;
    }
  }
  if (btnStable == LOW && !btnLongSent && (now - btnDownSince) >= LONG_PRESS_MS) {
    btnLongSent = true;
    return EVT_LONG;
  }
  return EVT_NONE;
}

void processInterfaceLoops() {
  ButtonEvent ev = pollButtonEvent();
  
  // Async monitoring injection for critical UI structures
  if (screen == SCREEN_RECORDING) {
    static uint32_t lastSync = 0;
    if (millis() - lastSync > 1000) { lastSync = millis(); syncCamStatus(); }
    if (ev == EVT_LONG) {
      sendCamCmd("STOP");
      char res[16];
      if (waitForCamResponse(res, sizeof(res), 1000)) {
        isRecordingActive = false;
        screen = SCREEN_CAMERA_MENU;
        beep(1000, 100);
      }
    }
    renderRecordingScreen();
    return;
  }

  switch (screen) {
    case SCREEN_EXO_SELECT:
      if (ev == EVT_SHORT) { exoCursor = (exoCursor + 1) % EXO_COUNT; beep(1000, 20); }
      if (ev == EVT_LONG) { cfg.exoType = exoCursor; saveConfig(); beep(1500, 60); screen = SCREEN_MAIN_MENU; }
      drawSelectionMenu("SELECT EXOSUIT ARTYPE", EXO_NAMES, EXO_COUNT, exoCursor);
      break;

    case SCREEN_MAIN_MENU:
      if (ev == EVT_SHORT) { mainCursor = (mainCursor + 1) % MAIN_COUNT; beep(1000, 20); }
      if (ev == EVT_LONG) {
        beep(1200, 40);
        if (mainCursor == 0) screen = SCREEN_STATUS;
        else if (mainCursor == 1) screen = SCREEN_SENSORS;
        else if (mainCursor == 2) { screen = SCREEN_CAMERA_MENU; cameraCursor = 0; syncCamStatus(); }
        else if (mainCursor == 3) { screen = SCREEN_SETTINGS; settingsCursor = 0; }
        else if (mainCursor == 4) { screen = SCREEN_EXO_SELECT; exoCursor = cfg.exoType; }
        else if (mainCursor == 5) screen = SCREEN_ABOUT;
      }
      drawSelectionMenu("MAIN TELEMETRY INDEX", MAIN_MENU, MAIN_COUNT, mainCursor);
      break;

    case SCREEN_STATUS:
      if (ev == EVT_SHORT) { statusPage = (statusPage + 1) % 2; beep(1000, 20); }
      if (ev == EVT_LONG) { beep(800, 40); screen = SCREEN_MAIN_MENU; }
      renderStatusScreen();
      break;

    case SCREEN_SENSORS:
      if (ev == EVT_SHORT) { sensorPage = (sensorPage + 1) % 4; beep(1000, 20); }
      if (ev == EVT_LONG) { beep(800, 40); screen = SCREEN_MAIN_MENU; }
      renderSensorsScreen();
      break;

    case SCREEN_CAMERA_MENU:
      if (ev == EVT_SHORT) { cameraCursor = (cameraCursor + 1) % CAMERA_COUNT; beep(1000, 20); }
      if (ev == EVT_LONG) {
        beep(1200, 40);
        if (cameraCursor == 0) { sendCamCmd("PHOTO"); beep(1800, 100); }
        else if (cameraCursor == 1) {
          sendCamCmd("REC");
          char res[16];
          if (waitForCamResponse(res, sizeof(res), 1000) && strcmp(res, "REC_START") == 0) {
            isRecordingActive = true;
            recordStartMs = millis();
            screen = SCREEN_RECORDING;
          }
        }
        else if (cameraCursor == 2) { syncCamStatus(); } 
        else if (cameraCursor == 3) { screen = SCREEN_DEVELOPER_MENU; devCursor = 0; } 
        else if (cameraCursor == 4) { screen = SCREEN_MAIN_MENU; }
      }
      {
        char camTitle[32];
        snprintf(camTitle, sizeof(camTitle), "CAM SUBPOS [%s]", camStatusStr);
        drawSelectionMenu(camTitle, CAMERA_MENU, CAMERA_COUNT, cameraCursor);
      }
      break;

    case SCREEN_SETTINGS:
      if (ev == EVT_SHORT) { settingsCursor = (settingsCursor + 1) % SETTINGS_COUNT; beep(1000, 20); }
      if (ev == EVT_LONG) {
        beep(1200, 40);
        if (settingsCursor == 0) { cfg.brightness = (cfg.brightness == 255) ? 64 : 255; setDisplayMaxBrightness(); saveConfig(); }
        else if (settingsCursor == 1) { cfg.bootAnim ^= 1; saveConfig(); }
        else if (settingsCursor == 2) { cfg.bootSound ^= 1; saveConfig(); }
        else if (settingsCursor == 3) { cfg.volume ^= 1; saveConfig(); }
        else if (settingsCursor == 4) { cfg.postDelay = (cfg.postDelay == 400) ? 100 : 400; saveConfig(); }
        else if (settingsCursor == 5) { 
          cfg.brightness = 255; cfg.bootAnim = 1; cfg.bootSound = 1; cfg.volume = 1; cfg.postDelay = 400; cfg.exoType = 0;
          saveConfig(); setDisplayMaxBrightness(); screen = SCREEN_EXO_SELECT;
        }
        else if (settingsCursor == 6) { screen = SCREEN_DEVELOPER_MENU; devCursor = 0; }
        else if (settingsCursor == 7) { screen = SCREEN_MAIN_MENU; }
      }
      {
        const char* settingsStrings[8];
        char b1[24], b2[24], b3[24], b4[24], b5[24];
        snprintf(b1, sizeof(b1), "Brightness: %s", (cfg.brightness == 255) ? "MAX" : "LOW");
        snprintf(b2, sizeof(b2), "Boot Anim: %s", cfg.bootAnim ? "ON" : "OFF");
        snprintf(b3, sizeof(b3), "Boot Sound: %s", cfg.bootSound ? "ON" : "OFF");
        snprintf(b4, sizeof(b4), "Volume: %s", cfg.volume ? "MAX" : "MUTE");
        snprintf(b5, sizeof(b5), "POST Del: %dms", cfg.postDelay);
        
        settingsStrings[0] = b1; settingsStrings[1] = b2; settingsStrings[2] = b3;
        settingsStrings[3] = b4; settingsStrings[4] = b5; settingsStrings[5] = "Factory Reset";
        settingsStrings[6] = "Developer Options"; settingsStrings[7] = "Back";
        drawSelectionMenu("SYSTEM CONTROL FIELD", settingsStrings, SETTINGS_COUNT, settingsCursor);
      }
      break;

    case SCREEN_DEVELOPER_MENU:
      if (ev == EVT_SHORT) { devCursor = (devCursor + 1) % DEV_COUNT; beep(1000, 20); }
      if (ev == EVT_LONG) {
        beep(1200, 40);
        if (devCursor == 0) screen = SCREEN_UART_MONITOR;
        else if (devCursor == 1) { runI2CScan(); screen = SCREEN_I2C_SCAN; scanPage = 0; }
        else if (devCursor == 2) screen = SCREEN_SENSOR_RAW;
        else if (devCursor == 3) screen = SCREEN_MEMORY_INFO;
        else if (devCursor == 4) screen = SCREEN_SETTINGS;
      }
      drawSelectionMenu("DEV CORE REGISTERS", DEV_MENU, DEV_COUNT, devCursor);
      break;

    case SCREEN_UART_MONITOR:
      if (ev != EVT_NONE) screen = SCREEN_DEVELOPER_MENU;
      drawHeader("UART RAW TERMINAL");
      display.setCursor(0, 12);
      display.printf("Cam Link: %s\n", camUartPresent ? "ONLINE" : "TIMEOUT");
      display.printf("Status Str: %s\n", camStatusStr);
      display.printf("Payload FPS: %.1f\n", camFPS);
      drawFooter("Press key to exit");
      display.display();
      break;

    case SCREEN_I2C_SCAN:
      if (ev == EVT_SHORT) { if (scanCount > 0) scanPage = (scanPage + 1) % ((scanCount + 3) / 4); }
      if (ev == EVT_LONG) screen = SCREEN_DEVELOPER_MENU;
      renderI2CScanScreen();
      break;

    case SCREEN_SENSOR_RAW:
      if (ev == EVT_LONG) screen = SCREEN_DEVELOPER_MENU;
      renderSensorRawScreen();
      break;

    case SCREEN_MEMORY_INFO:
      if (ev == EVT_LONG) screen = SCREEN_DEVELOPER_MENU;
      renderMemoryInfoScreen();
      break;

    case SCREEN_ABOUT:
      if (ev != EVT_NONE) screen = SCREEN_MAIN_MENU;
      renderAboutScreen();
      break;
      
    default:
      break;
  }
}

// =============================================================================
// MAIN FIRMWARE SETUP & EXECUTION STEP
// =============================================================================
void setup() {
  Serial.begin(921600);
  Serial1.begin(115200, SERIAL_8N1, UART1_RX, UART1_TX);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  buzzerInit();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true) delay(100);
  }
  
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.clearDisplay();
  display.display();

  runI2CScan();
  i2cOLEDPresent    = i2cPresent(OLED_ADDR);
  i2cEEPROMPresent  = i2cPresent(EEPROM24C32_ADDR);
  i2cBMP280Present  = i2cPresent(BMP280_ADDR);
  i2cBMP180Present  = i2cPresent(BMP180_ADDR);
  i2cMPU6050Present = i2cPresent(MPU6050_ADDR);
  eepromAvailable   = i2cEEPROMPresent;

  loadConfig();
  cfg.bootCount++;
  saveConfig();
  setDisplayMaxBrightness();

  bmp180LoadCalibration();
  bmp280LoadCalibration();
  mpu6050Init();

  if (cfg.bootAnim) {
    display.clearDisplay();
    display.drawBitmap(0, 0, epd_bitmap_images, 128, 64, SSD1306_WHITE);
    display.display();
    playIndustrialPostSound();
  }

  runVisualPOST();
  screen = SCREEN_EXO_SELECT;
}

void loop() {
  processInterfaceLoops();
  yield();
}