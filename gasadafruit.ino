#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "redmi4a"
#define WLAN_PASS       "qwertyuiop"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "guptavaibhav163"
#define AIO_KEY         "f7965f2a9651492fbaff6875fffc2de8"
int LED = 2;
int gas = D2;
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish GAS1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/GAS1");


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(gas, INPUT);
 
  // Connect to WiFi access point.
   //calling the connectToWifi() function
 connectToWifi();
}
int x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  if(WiFi.status() != WL_CONNECTED){
    connectToWifi();        //reconnect in case of connection loss
  }
  float temp = analogRead(D2);
  MQTT_connect();

  // Now we can publish stuff!
  Serial.print(F("\nSending Gas val "));
  Serial.print(x);
  Serial.print("...");
  if (! GAS1.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  int retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
void connectToWifi(){
  // funtion to connect to WiFi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("\n\nconnecting");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, !digitalRead(LED));   // blink internal LED
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nconnected: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED, HIGH);
}
