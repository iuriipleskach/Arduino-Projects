/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 */

#include <ESP8266WiFi.h>

const char* ssid = "ManUtd";
const char* password = "Bryant24";

int receivedCode = 0;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // Start the server
  server.begin();

  // Print the IP address
  // TODO: print to the LCD
//  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  client.flush();
  
  // Match the request
  int parametersBeginIndex = request.indexOf("?send=");
  int sendValue = 0;
  if (parametersBeginIndex != -1) {
    String parametersString = request.substring(parametersBeginIndex);
    int parameterValueBeginIndex = parametersString.indexOf("=") + 1;
    sendValue = parametersString.substring(parameterValueBeginIndex).toInt();
  }

  if (Serial.available() > 0) {
    receivedCode = Serial.parseInt();
  }

  Serial.print(sendValue);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
  s += request;
  s += "\r\n sender parameter = ";
  s += (sendValue);
  if (receivedCode != 0) {
    s += "\r\n receiver parameter = ";
    s += (receivedCode);
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
}


