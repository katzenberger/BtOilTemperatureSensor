/*
  Oil Temperature logger via Bluetooth with ESP32 module
  For Honda 2.2K NTC temperture sensor.
*/

#include <Thermistor.h>
#include <NTC_Thermistor.h>
#include <AverageThermistor.h>
#include <BluetoothSerial.h>

#define SENSOR_PIN             A0
#define REFERENCE_RESISTANCE   47000
#define NOMINAL_RESISTANCE     47000
#define NOMINAL_TEMPERATURE    25
#define B_VALUE                3950
#define ANALOG_RESOLUTION      4095

//#define USE_PIN
const char *pin = "1234";

String device_name = "BT-OilTemperature";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#define READINGS_NUMBER 10
#define DELAY_TIME 10

BluetoothSerial SerialBT;

NTC_Thermistor originThermistor(
  SENSOR_PIN,
  REFERENCE_RESISTANCE,
  NOMINAL_RESISTANCE,
  NOMINAL_TEMPERATURE,
  B_VALUE,
  ANALOG_RESOLUTION
);

AverageThermistor thermistor(
  &originThermistor,
  READINGS_NUMBER,
  DELAY_TIME
);

void setup() {
  Serial.begin(115200);

  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif

}

void loop() {
  const double celsius = thermistor.readCelsius();

  // Output to Serial
  Serial.print("Oil: ");
  Serial.print(celsius);
  Serial.println(" C");

  // Output to BT
  SerialBT.print("Oil: ");
  SerialBT.print(celsius);
  SerialBT.print(" C");
  SerialBT.println();
  
  delay(1000);
}
