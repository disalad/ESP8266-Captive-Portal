#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <pgmspace.h>

// User configuration
#define SSID_NAME "Free WiFi"
#define SUBTITLE "Free WiFi service."
#define TITLE "Sign in:"
#define BODY "Create an account to get connected to the internet."
#define POST_TITLE "Validating..."
#define POST_BODY "Your account is being validated. Please, wait up to 5 minutes for device connection.</br>Thank you."
#define PASS_TITLE "Credentials"
#define CLEAR_TITLE "Cleared"
#define WEB_TITLE "Sign in to use Free WiFi"

// Function prototypes
void readData();
void writeData(String data);
void deleteData();

// Init System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1);  // Gateway

String data = "";
String Credentials = "";
int savedData = 0;
int timer = 5000;
int i = 0;
unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String header(String t) {
  return "<!DOCTYPE html>"
         "<html lang=\"en\">"
         "<head>"
         "<meta charset=\"UTF-8\" />"
         "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />"
         "<title>Nemo Test</title>"
         "<style>"
         "@import url(\"https://fonts.googleapis.com/css2?family=Poppins:wght@500&display=swap\");"
         "</style>"
         "</head>"
         "<body>";
}

String footer() {
  return "</body></html>";
}

String index() {
  return header(WEB_TITLE) + footer();
}

String posted() {
  return header(POST_TITLE) + footer();
}

String creds() {
  return "<></>";
}

String clear() {
  return "<></>";
}

void setup() {
  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME);
  dnsServer.start(DNS_PORT, "*", APIP);  // DNS spoofing (Only HTTP)
  
  webServer.on("/post", []() {
    webServer.send(HTTP_CODE, "text/html", posted());
  });
  webServer.on("/creds", []() {
    webServer.send(HTTP_CODE, "text/html", creds());
  });
  webServer.on("/clear", []() {
    webServer.send(HTTP_CODE, "text/html", clear());
  });

  // Serve the index file
  webServer.onNotFound([]() {
    Serial.println("Sending web page...");
    webServer.send(200, "text/html", index());
  });

  webServer.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
}

void loop() {
  if ((millis() - lastTick) > TICK_TIMER) { lastTick = millis(); }
  dnsServer.processNextRequest();
  webServer.handleClient();

  i++;
  if (i == timer && savedData == 1) {
    i = 0;
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
  }
  if (i > timer) { i = 0; }
}
