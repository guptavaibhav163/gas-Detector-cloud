#include <ESP8266WiFi.h>

#define WIFI_SSID     "redmi4a" // your wifi SSID name
#define WIFI_PASSWORD "qwertyuiop" // wifi pasword
String apiWritekey = "7H8VO6YJWXSU57DM"; // replace with your THINGSPEAK WRITEAPI key here
 
const char* server = "api.thingspeak.com";
int LED = 2;
int gas = A0;
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(gas, INPUT);
  
  //calling the connectToWifi() function
  connectToWifi();
}
 
void loop() {
   if(WiFi.status() != WL_CONNECTED){
    connectToWifi();        //reconnect in case of connection loss
  }
  float temp = analogRead(D2);
  
  if (client.connect(server,80))
  {  
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(temp);
           tsData += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
     client.print(tsData);
 
     Serial.print("GAS  : ");
     Serial.print(temp);
     Serial.println("uploaded to Thingspeak server....");
  }
  client.stop();
 
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  // thingspeak needs minimum 15 sec delay between updates
  delay(10000);
}
void connectToWifi(){
  // funtion to connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
