/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
 const char* UBIDOTS_TOKEN = "BBFF-a6fLP4ohBpW1a9B7GaBy2QnN9wqlGA";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "Kemal";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "qwertyuiop";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "Smart_Agri";   // Put here your Device label to which data  will be published
const char *LABEL_LDR = "Cahaya";
const char *LABEL_SOIL = "Tanah";
const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;
uint8_t ldrpin = 34; // Pin used to read data from GPIO34 ADC_CH6.
uint8_t soilpin= 33; // Pin used to read data from GPIO34 ADC_CH6.

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();

  timer = millis();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if ((millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    int lux = analogRead(ldrpin);
   int moisturePercentage =analogRead(soilpin); 
   //( 100 - ( (analogRead(soilpin) / 1023.00) * 100 ) );
  Serial.print(moisturePercentage);
    ubidots.add(LABEL_SOIL, moisturePercentage);
     ubidots.add(LABEL_LDR, lux);// Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  ubidots.loop();
  delay(500);
}
