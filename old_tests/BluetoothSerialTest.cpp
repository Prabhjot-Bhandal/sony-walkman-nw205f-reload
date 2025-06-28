//BLUETOOTH CLASSIC NOT SUPPORTED ON ESP32-S3!!

//#include <Arduino.h>
//This is for testing OUTPUT bluetooth signals with ESP32
//Includes the Bluetooth Serial library
#include <BluetoothSerial.h>

//This is a preprocessor if statement, ensures that everything is ready before running the main function
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is NOT enabled!
#endif

//Makes a BluetoothSerial object instance
BluetoothSerial SerialBT;

void setupBTObject() {
    //Starts bluetooth signal on the ESP32 at a frequency of 115200 Hz
    Serial.begin(115200);
    //Names the Bluetooth object
    SerialBT.begin("Amogus");
    Serial.println("Your Bluetooth device is ready to pair");
}

void checkForBT() {
    //If data is being received by the Serial port, that data is sent to a Bluetooth connected device
    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }

    //If data is received via Bluetooth, the data is written on the Serial Monitor
    if (SerialBT.available()) {
        Serial.write(SerialBT.read());
    }
}

void setup() {
  setupBTObject();
}

void loop() {
  checkForBT();
}