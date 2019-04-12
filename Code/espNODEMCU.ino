#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <WiFiUDP.h>

WiFiClient espClient;
PubSubClient client(espClient);


#define WIFI_SSID "House LANnister"
#define WIFI_KEY "superpen15"

#define HOST "192.168.0.11" //IP of BeagleBone
#define PORT 1883 //port for UDP
#define DTIME 1000 //delay time

#define mqtt_server "localhost"
#define topic "wash"

void setup() {
Serial.begin(9600);

client.setServer(HOST, PORT); //connect to server

// Connect to WiFi
Serial.println();
Serial.print("Connecting to ");
Serial.println(WIFI_SSID);
// begin the wifi connection
WiFi.begin(WIFI_SSID, WIFI_KEY);
//wait for wifi to finish connecting
while (WiFi.status() != WL_CONNECTED) { //while not connected
delay(500);
Serial.print("."); //print dots to show activity
}
Serial.println(""); //linefeed
Serial.println("WiFi connected"); //display connection message

// Start the server
Serial.println("Server started");

// Print the IP address
Serial.println(WiFi.localIP());
}


void loop() {
int vibrationvalue=0 ;
while (vibrationvalue <50){ //while low vibration wait
  Serial.print("Sleeping\n");
  delay(DTIME);//in ms
  sensor_average(vibrationvalue);
  Serial.println(vibrationvalue);
 }

while (vibrationvalue >= 50){ //while vibration high wait
   Serial.println("Running");
   delay(DTIME);
  sensor_average(vibrationvalue);
  Serial.println(vibrationvalue);
 }
Serial.println("Laundery Done!"); //vibration has stopped
mqttpub(); //run mosquitto publish

}
void mqttpub()
{
if(client.connect("ESP8266Client")) //set client
  Serial.println("connected");
Serial.println("loop");
if(client.loop()) //if connected
  {
  Serial.println("publishing");
  client.publish(topic, "wash", true); //send wash
  Serial.println("Publishing done");
  }
else
  Serial.println("error not connected");
}
void sensor_average(int & vibrationvalue){
  int temp;
  vibrationvalue =0; //average 10 values over 100ms
  for(int i=0;i<10;i++)
    {
    delay(10);
    temp = analogRead(A0);
    vibrationvalue += temp;
    }
    vibrationvalue/=10;
}

