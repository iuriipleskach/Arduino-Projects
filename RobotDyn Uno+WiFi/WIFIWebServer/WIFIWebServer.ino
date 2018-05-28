/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 */

#include <ESP8266WiFi.h>

const char* ssid = "ManUtd";
const char* password = "Bryant24";

unsigned long receivedCode = 0;
WiFiServer server(80);

// TODO: next steps:
// - Implement handling different IR types, implement serialization for IR decode_results on the client side?
// - Try to connect LCD to show local IP, so it will be easier to join on networks we don't have admin access to specify static IP address.
// - Implement array of all (or limited number but more than 1) values received from Serial?


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
  parseSerialMessage();

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

  String body = "";
  if (sendRequested(request)) {
    unsigned long sendValue = parseParameterValueFromString(request, "send");
    Serial.print(sendValue);

    body = "\r\n Sending: " + sendValue;
  } else if (receiveRequested(request)) {
    // TODO: implement short and verbose modes. Short can be used for mobile app, verbose for debugging in the browser
    body =  receivedCode != 0 ? "\r\n Last received: " + (receivedCode) : "No code received";
  } else {
    client.stop();
  }

  // Send the response to the client
  client.print(htmlResponse(body));
  delay(1);
}

void parseSerialMessage() {
  // TODO: parse comma separated IR decode_results
  if (Serial.available() > 0) {
    receivedCode = Serial.parseInt();
  }
}

String htmlResponse(String body) {
  String htmlResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
  htmlResponse += body;
  htmlResponse += "</html>\n";
  return htmlResponse;
}

bool sendRequested(String string) {
  return string.indexOf("?send=") != -1;
}

bool receiveRequested(String string) {
  return string.indexOf("?receive") != -1;
}

unsigned long parseParameterValueFromString(String string, String parameterName) {
  int parametersBeginIndex = string.indexOf("?" + parameterName + "="); // ?send=, ?receive=
  unsigned long value = 0;
  if (parametersBeginIndex != -1) {
    String parametersString = string.substring(parametersBeginIndex);
    int parameterValueBeginIndex = parametersString.indexOf("=") + 1;
    value = parametersString.substring(parameterValueBeginIndex).toInt();
  }
  return value;
}
