//BLUETOOTH CLASSIC NOT SUPPORTED ON ESP32-S3!!

//Library for A2DP as a Bluetooth Source
#include "BluetoothA2DPSource.h"
#include <Arduino.h> //For math
#include "nvs_flash.h" //For erasing flash memory

//Creates an A2DP Source object, basically creates the sender
BluetoothA2DPSource a2dp_source;

#define standard_frequency 440.0 //Hz

/*Callback Function: Generates the audio stream to the bluetooth audio device, kinda like getting frames for a screen to output.
  Frame: An object that holds an amplitude/displacement value for the speaker. It stores 2 values for 2 channels of audio, left and right.
  Return Type: int32_t, the number of frames of audio that will be provided.
  Parameters: Frame *frame, an object that stores audio data for 2 channels.
              in32_t frame_count, a positive integer that contains the number of frames per sample of audio
              Samples = Portions of time that are */
int32_t get_audio_frames(Frame *frame, int32_t frame_count) {
  //LOCAL VARIABLES
  static float m_time = 0.0; //Current time-value in the sine wave
  float m_amplitude = 10000.0;  //Amplitude of the sine wave
  float m_deltaTime = 1.0 / 44100.0; //The length/time between each sample of audio at a 44.1kHz sample rate (22.7 ms)
  float m_phase = 0.0; //Offsets the wave, the horizontal shift
  float pi_2 = PI * 2.0;

  //Loop through all audio frames in the current audio sample
  for (int sample = 0; sample < frame_count; ++sample) {
    //Determines the angle/argument for the sine function based on the current time value and frequency of the music note
    float angle = pi_2 * standard_frequency * m_time + m_phase;
    //The same sine value is assigned to both the left and right channels, multiplied by the amplitude
    frame[sample].channel1 = m_amplitude * sin(angle);
    frame[sample].channel2 = frame[sample].channel1;
    //Moves to the next audio sample
    m_time += m_deltaTime;

  }//For loop

  //Prevents the code from stopping from watchdog
  delay(1);

  //Returns how many frames that will be played in the current audio sample
  return frame_count;

} //get_audio_data_callback()

void setup() {
  //Starts serial monitor at 115200 baud-rate
  Serial.begin(115200);
  delay(1000);

  //Initialize NVS Flash Memory
  esp_err_t ret = nvs_flash_init();
  //Checks for NVS Flash corruption or if memory full
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    Serial.println("NVS corrupted or full. Erasing...");
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret); //Crash if it still fails

  //Disables ability for last Bluetooth device to automatically reconnect
  a2dp_source.set_auto_reconnect(false);
  //Tells the Bluetooth object to get audio frames to play
  a2dp_source.set_data_callback_in_frames(get_audio_frames);
  //Sets the volume for the bluetooth stream (range of 0 to 100)
  a2dp_source.set_volume(30);  

  //Attempts to connect to a device with the name "Soundcore Life Q35"
  a2dp_source.start("Soundcore Life Q35");
  
  delay(1000);

  //If direct connection is needed, must put device MAC address in 8-bit integers
  //uint8_t q35_mac[] = { 0xE8, 0xEE, 0xCC, 0xD0, 0x63, 0x97 };
  //a2dp_source.connect_to(q35_mac);

} //setup()

void loop() {

  //Prevents code from stopping from watchdog
  delay(1000);

} //loop()