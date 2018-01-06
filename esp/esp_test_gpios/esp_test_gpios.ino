/*
 *  EasyloT sketch tweeked by Barnabybear - original @
 *  http://iot-playground.com/2-uncategorised/40-esp8266-wifi-relay-switch-arduino-ide
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

const char* ssid = "your-ssid";
const char* password = "your-password";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started @ ");

  // Print the IP address & instructions
  Serial.println(WiFi.localIP());
  Serial.println("To control GPIO, open your web browser.");
  Serial.println("To set GPIO 2 high, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/1");
  Serial.println("To set GPIO 2 low, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/0");
  Serial.println("To toggle GPIO 2, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/2");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    delay(100);
    return;
  }
  
  // Commented out by BB as gets stuck in while loop.
  // Wait until the client sends some data
  //Serial.println("new client");
  //while(!client.available()){
  //  delay(1);
  //}
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else if (req.indexOf("/gpio/2") != -1)
   val = (!digitalRead(2)); // <<<<< Edit: insert /gpio/3 lines after this line.
  else {
    Serial.println("invalid request");
    client.print("HTTP/1.1 404\r\n");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
