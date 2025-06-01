#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h> 

// pines de sensores y actuadores
#define DHTPIN 2
#define DHTTYPE DHT22
#define LDRPIN A1
#define ONE_WIRE_BUS 3
#define SERVO_PIN 6
#define LED_ALERTA 13

// pines del registro 74HC595
const int latchPin = 12;
const int clockPin = 11;
const int dataPin  = 10;

// objetos de librerías
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo valvula;

// hace media de varias lecturas de temperatura
float readAverageTemperature(int samples = 5) {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    float temp = dht.readTemperature();
    if (!isnan(temp)) {
      sum += temp;
    }
    delay(200); // entre lecturas
  }
  return sum / samples;
}

// controla el servo segun la temperatura de la bateria
void controlTemperatura(float t_bateria) {
  float setPoint = 25.0; // ideal
  float zona = 3.0;      // margen

  if (t_bateria < setPoint - zona) {
    valvula.write(0); // calentar
  } else if (t_bateria > setPoint + zona) {
    valvula.write(180); // enfriar
  } else {
    valvula.write(90); // neutro
  }
}

// controla los leds en base a la luz que mide el LDR
void controlIluminacion(int ldr_value) {
  int nivel = map(ldr_value, 0, 1015, 8, 0); // cuanto más oscuro, más leds

  byte salida = 0;
  for (int i = 0; i < nivel; i++) {
    salida |= (1 << i); // activa cada led
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, salida);
  digitalWrite(latchPin, HIGH);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensors.begin();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Iniciando...");
  delay(1500);
  lcd.clear();

  valvula.attach(SERVO_PIN);
  valvula.write(90); // empieza en neutro

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);

  pinMode(LED_ALERTA, OUTPUT);
  digitalWrite(LED_ALERTA, LOW);
}

void loop() {
  float t = readAverageTemperature();
  float h = dht.readHumidity();
  sensors.requestTemperatures();
  float t_ds18b20 = sensors.getTempCByIndex(0);
  int ldr_value = analogRead(LDRPIN);

  // si hay error en el sensor de temperatura/humedad
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer DHT");
    lcd.clear();
    lcd.print("Error sensor DHT");
    delay(2000);
    return;
  }

  // si la temperatura de la bateria es muy baja
  if (t_ds18b20 < 0) {
    digitalWrite(LED_ALERTA, HIGH);
    lcd.clear();
    lcd.print("No cargar bat.");
    lcd.setCursor(0, 1);
    lcd.print("Temp baja!");
    delay(3000);
  } else {
    digitalWrite(LED_ALERTA, LOW);

    // apaga o enciende backlight segun la luz
    if (ldr_value > 800) {
      lcd.noBacklight();
    } else {
      lcd.backlight();
    }

    controlTemperatura(t_ds18b20);
    controlIluminacion(ldr_value);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t, 1);
    lcd.print("C H:");
    lcd.print(h, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Luz:");
    lcd.print(ldr_value);
    lcd.print(" T2:");
    lcd.print(t_ds18b20, 0);
  }

  Serial.print("Temp DHT: ");
  Serial.print(t);
  Serial.print(" °C | Hum: ");
  Serial.print(h);
  Serial.print("% | LDR: ");
  Serial.print(ldr_value);
  Serial.print(" | Temp DS18B20: ");
  Serial.println(t_ds18b20);

  delay(4000);
}

