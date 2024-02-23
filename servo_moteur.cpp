/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 13.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 Circuit:
 * WiFi shield attached
 * LED attached to pin 13

 created 25 Nov 2012
 by Tom Igoe
 modified 16 Sept 2015
 by Ng Beng Chet
 */
 
#include <CytronWiFiShield.h>
#include <CytronWiFiServer.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#define WiFi wifi

Servo monServomoteur;

int servoPin = 9;

char ssid[] = "Iphone de Kevin";      //  your network SSID (name)
char pass[] = "kevin2004";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

ESP8266Server server(80);
bool status = false;
int position=0;
int positionchange=0;
void setup() {
  Serial.begin(115200);      // initialize serial communication
  pinMode(5, OUTPUT);      // set the LED pin mode
  monServomoteur.attach(9);
  // check for the presence of the shield:
  if (!WiFi.begin(2, 3)) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

  String fv = WiFi.firmwareVersion();
  Serial.println(fv);

  // attempt to connect to Wifi network:
  while (!status) {
    
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);
    
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.connectAP(ssid, pass);
  }
  
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  
  ESP8266Client client = server.available();   // listen for incoming clients

  if(!client) return;
  Serial.println("new client");
  
  if (client.connected()) //if client is present and connected
  {             
      String s = client.readStringUntil('\r');   //get the first line of request       
      // Check to see if the client request was "GET /H" or "GET /L":
      
      
      if (strstr(s.c_str(),"GET /Debut")){ 
        position=0;               // GET /H turns the LED on
        
      }else if (strstr(s.c_str(),"GET /milieu")) {
        position=90;               // GET /H turns the LED on
         
      }else if (strstr(s.c_str(),"GET /fin")){
        position=180;

      }if (position !=positionchange ) {
    // Mettre à jour la position actuelle
    positionchange=position;

    // Effectuer l'action (déplacer le servomoteur, dans votre cas)
    monServomoteur.write(position);}

    // Mettre à jour la position de changement
    

      delay(10000);
      Serial.print(s);
      while(client.available())      
        Serial.write(client.read());                    // print the client request out the serial monitor

      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
      // and a content-type so the client knows what's coming, then a blank line:
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println();

      // the content of the HTTP response follows the header:
      
      client.print("Click <a href=\"/Debut\">here</a> position initiale<br>");
      client.print("Click <a href=\"/fin\">here</a> position finale<br>");
      client.print("Click <a href=\"/milieu\">here</a> position milieu<br>");
      

      // The HTTP response ends with another blank line:
      client.println();
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

