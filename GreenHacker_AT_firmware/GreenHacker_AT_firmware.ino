// Demo Code for SerialCommand Library
// Steven Cogswell
// May 2011
// Heavily modified by Dr. Aung Win Htut to Creat AT command firmware for ESP8266

#include <SerialCommand.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>


#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#define arduinoLED 2   // Arduino LED on board

SerialCommand sCmd;     // The demo SerialCommand object

/* Set these to your desired credentials. */
const char *ssid = "ESPbluephoenix";
const char *password = "bluephoenix";

ESP8266WebServer server(80);







/////////////////////

void AT() {
  Serial.println();
  Serial.println("AT OK");
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}



// AT+CWJAP="SSID", "password"       // SSID and password of router  Response :OK 
void AT_CWJAP(){
  Serial.println();
  Serial.println("AT+CWJAP OK");
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}

// Response :192.168.3.106   // Device got an IP from router.
void AT_CIFSR(){
  Serial.println();
  Serial.println("AT+CIFSR OK");
  Serial.print("Local Wifi ip (Station)  :: ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi AP ip (Acces Point) :: ");
  Serial.println(WiFi.softAPIP());
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}

void AT_RST(){
  Serial.println();
  Serial.println("Restarting.......");
  ESP.restart();  
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}

void AT_GMR(){
  Serial.println();
  Serial.println("AT+GMR OK");
  Serial.println("Green Hacker AT firmware version 1.0a");
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}

void AT_CWMODEQ(){
  Serial.println();
  Serial.println("AT+CWMODE OK");
  Serial.println(WiFi.status());
  WiFi.printDiag(Serial);
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}

//Wireless mode, AP, Station, AP+Station AT+CWMODE=3      // softAP+station mode  Response :OK
void AT_STACWMODE(){
  Serial.println();
  Serial.println("AT+STACWMODE OK");
  WiFi.mode(WIFI_STA);  
  WiFi.printDiag(Serial);
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}
void AT_APCWMODE(){
  Serial.println();
  Serial.println("AT+APCWMODE OK");  
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 
  WiFi.mode(WIFI_AP);
  WiFi.printDiag(Serial);
  digitalWrite(arduinoLED, HIGH);
  delay(500);
  digitalWrite(arduinoLED, LOW);
}
////////////////////
void LED_on() {
  Serial.println("LED on");
  digitalWrite(arduinoLED, HIGH);
}


void LED_off() {
  Serial.println();
  Serial.println("LED off");
  digitalWrite(arduinoLED, LOW);
}

void sayHello() {
  Serial.println();
  char *arg;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL) {    // As long as it existed, take it
    Serial.print("Hello ");
    Serial.println(arg);
  }
  else {
    Serial.println("Hello, whoever you are");
  }
}


void processCommand() {
  int aNumber;
  char *arg;
  Serial.println();
  Serial.println("We're in processCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No arguments");
  }

  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atol(arg);
    Serial.print("Second argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No second argument");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.println("What?");
}


void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}


void setup() {
  pinMode(arduinoLED, OUTPUT);      // Configure the onboard LED for output
  digitalWrite(arduinoLED, LOW);    // default to LED off
  Serial.begin(9600);
  
  // Setup callbacks for SerialCommand commands
  //AT commands
  sCmd.addCommand("AT",AT);
  sCmd.addCommand("AT+CWJAP",AT_CWJAP);       // AT+CWJAP="SSID", "password"       // SSID and password of router  Response :OK 
  sCmd.addCommand("AT+CIFSR",AT_CIFSR);       // Response :192.168.3.106   // Device got an IP from router.
  sCmd.addCommand("AT+RST",AT_RST);
  sCmd.addCommand("AT+GMR",AT_GMR);
  sCmd.addCommand("AT+CWMODE?",AT_CWMODEQ);
  sCmd.addCommand("AT+STACWMODE",AT_STACWMODE);     //Wireless mode, AP, Station, AP+Station AT+CWMODE=3      // softAP+station mode  Response :OK
  sCmd.addCommand("AT+APCWMODE",AT_APCWMODE);
  //sCmd.addCommand("AT",AT);
  //sCmd.addCommand("AT",AT);
  //sCmd.addCommand("AT",AT);
  //sCmd.addCommand("AT",AT);


  
  sCmd.addCommand("ON",    LED_on);          // Turns LED on
  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
  sCmd.addCommand("HELLO", sayHello);        // Echos the string argument back
  sCmd.addCommand("P",     processCommand);  // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")

/////////////////////////////////////////////

  Serial.println("");
  Serial.println("Green Hacker Team");
  Serial.println("Leader: Dr. Aung Win Htut");
  Serial.println("AT Like Firmware for ESP8266");
  Serial.println("Ready to use Now");
  Serial.print("ESP8266 chip ID is             :: ");
  Serial.println(ESP.getChipId());
  //Serial.print("Last reset reason is :: ");
  //Serial.println(ESP.getResetReason());
  Serial.print("Free heap size is              :: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Flash chip ID is               :: ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("Flash chip SIZE is             :: ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("Flash chip Speed is            :: ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("CPU instruction cycle count is :: ");
  Serial.println(ESP.getCycleCount());
  //Serial.print("Supply Voltage (VCC) is :: ");
  //Serial.println(ESP.getVcc()); //add first      ADC_MODE(ADC_VCC);
  
  
/////////////////////////////////////////////////
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(1000);
////////////////////////////////////////////////////////
  WiFi.mode(WIFI_AP_STA);
  Serial.println();
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  /////////////////////////////////////////////////////
  int i=0;
  for(i=0;i<10;i++)
  {
    Serial.print("#");
    delay(200);
  }

  Serial.println("");
}

void loop() {
  sCmd.readSerial();     // We don't do much, just process serial commands
  server.handleClient();
}
