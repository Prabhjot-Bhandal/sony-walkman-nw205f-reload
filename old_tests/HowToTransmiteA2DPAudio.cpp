//BLUETOOTH CLASSIC NOT SUPPORTED ON ESP32-S3!!

//Library for A2DP as a Bluetooth Source
#include "BluetoothA2DPSource.h"
#include <Arduino.h>

//Creates an A2DP Source object, basically creates the sender
BluetoothA2DPSource a2dp_source;

#define c3_frequency 130.81 //Hz, C3 Musical Note, like Tuning Forks, tone will have 130.81 cycles/second

/*Callback Function: Generates the audio stream to the bluetooth audio device, kinda like getting frames for a screen to output.
  Frame: An object that holds an amplitude/displacement value for the speaker. It stores 2 values for 2 channels of audio, left and right.
  Return Type: int32_t, the number of frames of audio that will be provided.
  Parameters: Frame *frame, an object that stores audio data for 2 channels.
              in32_t frame_count, a positive integer that contains the number of frames per sample of audio
              Samples = Portions of t*/
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
    float angle = pi_2 * c3_frequency * m_time + m_phase;
    //The same sine value is assigned to both the left and right channels, multiplied by the amplitude
    frame[sample].channel1 = m_amplitude * sin(angle);
    frame[sample].channel2 = frame[sample].channel1;
    //Moves to the next audio sample
    m_time += m_deltaTime;

  }//For loop

  //Prevents the code from stopping
  delay(1);

  //Returns how many frames that will be played in the current audio sample
  return frame_count;

} //get_audio_data_callback()


void setup() {
  //Disables ability for last Bluetooth device to automatically reconnect
  a2dp_source.set_auto_reconnect(false);
  //Tells the Bluetooth object to get audio frames to play
  a2dp_source.set_data_callback_in_frames(get_audio_frames);
  //Sets the volume for the bluetooth stream (range of 0 to 100)
  a2dp_source.set_volume(30);  
  //Names itself and becomes discoverable
  a2dp_source.start("LEXON MINO L");

} //setup()

void loop() {

  //Prevents code from stopping
  delay(1000);

} //loop()