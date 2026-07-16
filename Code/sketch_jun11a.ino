#define BLYNK_TEMPLATE_ID   "TMPL6_CLaR165"
#define BLYNK_TEMPLATE_NAME "Smart Energy Meter"
#define BLYNK_AUTH_TOKEN    "RTJaF7jbiS18O1wtAgx8XexC2qEFSLzV"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CURRENT_SENSOR_PIN A0
#define AC_RELAY_PIN       D3
#define DC_RELAY_PIN       D6
#define DC_SENSE_PIN       D7

LiquidCrystal_I2C lcd(0x27, 16, 2);

char ssid[] = "UCHIHA";
char pass[] = "123456789";

float balance = 100.0;
float current_amps = 0.0;
float power_watts = 0.0;
const float vout_per_amp = 0.185;
unsigned long last_update = 0;
String current_source = "NONE";
String last_source = "NONE";  // ← আগের source track করবে

BLYNK_WRITE(V0) {
  float recharge_amount = param.asFloat();
  balance += recharge_amount;
  Blynk.virtualWrite(V1, balance);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(AC_RELAY_PIN, OUTPUT);
  pinMode(DC_RELAY_PIN, OUTPUT);
  pinMode(DC_SENSE_PIN, INPUT);

  digitalWrite(AC_RELAY_PIN, HIGH);
  digitalWrite(DC_RELAY_PIN, HIGH);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.persistent(false);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(ssid, pass);

  int wifi_tries = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_tries < 60) {
    delay(500);
    Serial.print("Status: ");
    Serial.println(WiFi.status());
    wifi_tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected!");
    Serial.println(WiFi.localIP());
    lcd.setCursor(0, 1);
    lcd.print("WiFi OK!        ");
    delay(500);

    Blynk.config(BLYNK_AUTH_TOKEN);
    Blynk.connect(5000);

    if (Blynk.connected()) {
      Serial.println("Blynk Connected!");
    } else {
      Serial.println("Blynk FAILED!");
    }

  } else {
    Serial.println("WiFi FAILED!");
    lcd.setCursor(0, 1);
    lcd.print("WiFi FAILED!    ");
    delay(1000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hybrid Energy");
  lcd.setCursor(0, 1);
  lcd.print("Meter Ready!    ");
  delay(1000);
  lcd.clear();
}

void switchToAC() {
  if (last_source != "AC MAIN") {
    Serial.println("Switching to AC...");
    digitalWrite(DC_RELAY_PIN, HIGH);  // আগে DC বন্ধ
    delay(300);                         // ৩০০ms অপেক্ষা — spike settle হতে দাও
    digitalWrite(AC_RELAY_PIN, LOW);   // তারপর AC চালু
    last_source = "AC MAIN";
    Serial.println("AC ON");
  }
  current_source = "AC MAIN";
}

void switchToDC() {
  if (last_source != "SOLAR") {
    Serial.println("Switching to DC...");
    digitalWrite(AC_RELAY_PIN, HIGH);  // আগে AC বন্ধ
    delay(300);                         // ৩০০ms অপেক্ষা
    digitalWrite(DC_RELAY_PIN, LOW);   // তারপর DC চালু
    last_source = "SOLAR";
    Serial.println("DC ON");
  }
  current_source = "SOLAR";
}

void switchOff() {
  if (last_source != "CUT-OFF") {
    Serial.println("CUT-OFF!");
    digitalWrite(AC_RELAY_PIN, HIGH);
    digitalWrite(DC_RELAY_PIN, HIGH);
    last_source = "CUT-OFF";
  }
  current_source = "CUT-OFF";
}

void loop() {
  // ── Blynk সবসময় চলবে ──
  Blynk.run();

  // ── Disconnect হলে reconnect ──
  if (!Blynk.connected() && WiFi.status() == WL_CONNECTED) {
    static unsigned long next_connect = 0;
    if (millis() > next_connect) {
      Serial.println("Reconnecting Blynk...");
      Blynk.connect(1000);
      next_connect = millis() + 15000;
    }
  }

  // ── Non-blocking current sampling ──
  static unsigned long sample_start = 0;
  static float current_sum = 0;
  static int read_count = 0;

  if (millis() - sample_start < 20) {
    int raw_analog = analogRead(CURRENT_SENSOR_PIN);
    float voltage = (raw_analog / 1024.0) * 3.3 * 1.515;
    float sample_current = (voltage - 2.5) / vout_per_amp;
    current_sum += (sample_current * sample_current);
    read_count++;
  } else if (read_count > 0) {
    current_amps = sqrt(current_sum / read_count);
    if (current_amps < 0.15) current_amps = 0.0;
    current_sum = 0;
    read_count = 0;
    sample_start = millis();
  } else {
    sample_start = millis();
  }

  // ── প্রতি ১ সেকেন্ডে মূল লজিক ──
  if (millis() - last_update >= 1000) {
    int dc_available = digitalRead(DC_SENSE_PIN);

    if (dc_available == HIGH) {
      power_watts = current_amps * 12.0;
    } else {
      power_watts = current_amps * 220.0;
    }

    if (power_watts > 0 && balance > 0) {
      float cost_per_second = (power_watts / 1000.0) * (7.0 / 3600.0);
      balance -= cost_per_second;
      if (balance < 0) balance = 0;
    }

    // ── Relay switching ──
    if (dc_available == HIGH) {
      switchToDC();
    } else {
      if (balance > 0) {
        switchToAC();
      } else {
        switchOff();
      }
    }

    update_display();
    last_update = millis();
  }
}

void update_display() {
  lcd.setCursor(0, 0);
  lcd.print("Pwr:" + String(power_watts, 1) + "W " + current_source + "   ");
  lcd.setCursor(0, 1);
  lcd.print("Bal:" + String(balance, 2) + "Tk    ");

  if (Blynk.connected()) {
    Blynk.virtualWrite(V1, balance);
    Blynk.virtualWrite(V2, power_watts);
    Blynk.virtualWrite(V3, current_amps);
    Blynk.virtualWrite(V4, current_source);
  }
}