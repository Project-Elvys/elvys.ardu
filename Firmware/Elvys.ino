#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASSWORD"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

WiFiUDP Udp;

// Local port to listen on
const uint16_t localUdpPort = 34232;  
// Buffer for incoming packets
char incomingPacket[255];  

void setup() {
  Serial.begin(115200);

  // Start connecting to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  delay(500);

  Serial.println("");
  Serial.println("Waiting for WiFi... ");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  Udp.begin(localUdpPort);
  Serial.printf("Listening on IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  pinMode(D5, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);    // HIGH setting on this pin turns the LED off
  digitalWrite(D5, LOW);

}


void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // Receive UDP packet and display content
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // Turn on LED & power optocoupler 
    digitalWrite(LED_BUILTIN, LOW);  
    digitalWrite(D5, HIGH);
    delay(1500);
    
    // Turn off LED & unpower optocoupler 
    digitalWrite(LED_BUILTIN, HIGH);    
    digitalWrite(D5, LOW);
    delay(1000);
  }
}
