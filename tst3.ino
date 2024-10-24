#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "DISPOSITIVOS";
const char* password = "Sp33dc@st99";
//const char* ssid = "SPEEDCAST - OPERACIONAL";
//const char* password = "Ac3ss02019";
//const char* ssid = "EUTELSAT";
//const char* password = "eutelsat";


#define DHTPIN1 D1  
#define DHTPIN2 D2  
#define DHTPIN3 D3  
#define DHTPIN4 D4  
#define DHTPIN5 D5  
#define DHTPIN6 D6  

#define DHTTYPE DHT11


DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
DHT dht5(DHTPIN5, DHTTYPE);
DHT dht6(DHTPIN6, DHTTYPE);


float t1 = 0.0;
float t2 = 0.0;
float t3 = 0.0;
float t4 = 0.0;
float t5 = 0.0;
float t6 = 0.0;

//IPAddress ip(192, 168, 111, 242);
//IPAddress gateway(192, 168, 111, 1);
//IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80);

unsigned long previousMillis = 0;
const long interval = 10000;  

void setup() {
    Serial.begin(115200);
    dht1.begin();
    dht2.begin();
    dht3.begin();
    dht4.begin();
    dht5.begin();
    dht6.begin();

    WiFi.begin(ssid, password);
    Serial.println("Conectando ao WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println(".");
    }
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><head>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>"
                  "body { font-family: Arial; margin: 0; }"
                  ".container { display: flex; justify-content: space-between; }"
                  ".left { flex: 1; text-align: left; padding: 20px; }"
                  ".right { flex: 1; text-align: left; padding: 20px; }"
                  "h2 { font-size: 3.0rem; text-align: center; }"
                  "h3 { font-size: 2.5rem; }"
                  "p { font-size: 1.5rem; margin: 20px 0; }"
                  ".center { text-align: center; }" 
                  "</style></head><body>"
                  "<h2>Temperature SPEEDCAST</h2>"
                  "<p class=\"center\">Updated in real time</p>"
                  "<div class=\"container\">"
                  "<div class=\"left\">"
                  "<ul>"
                  "<h3>Room</h3>"
                  
                  "<li><p><span class=\"dht-labels\">Room 1:</span> <span id=\"temperature1\">" + String(t1) + " &deg;C</span></p></li>"
                  "<li><p><span class=\"dht-labels\">Room 2:</span> <span id=\"temperature2\">" + String(t2) + " &deg;C</span></p></li>"
                  "<li><p><span class=\"dht-labels\">Room 3:</span> <span id=\"temperature3\">" + String(t3) + " &deg;C</span></p></li>"
                  
                  "<h3>Output of the Air</h3>"
                  
                  "<li><p><span class=\"dht-labels\">Out 1:</span> <span id=\"temperature4\">" + String(t4) + " &deg;C</span></p></li>"
                  "<li><p><span class=\"dht-labels\">Out 2:</span> <span id=\"temperature5\">" + String(t5) + " &deg;C</span></p></li>"
                  "<li><p><span class=\"dht-labels\">Out 3:</span> <span id=\"temperature6\">" + String(t6) + " &deg;C</span></p></li>"
                  "</ul>"
                  "</div>"
                  "<div class=\"right\">"
                  "<h3>information</h3>"
                 "<ul style=\"font-size: 14px;\">"
                  "<li><p style=\"font-size: 1.0rem;\">To see the API in mode, just type '/api' at the end of the ip.</p>"
                  "<li><p style=\"font-size: 1.0rem;\">To see only the temperature of room 1, simply type '/api/room/1' at the end of the ip.</p></li>"
                  "<li><p style=\"font-size: 1.0rem;\">To see only the temperature of room 2, simply type '/api/room/2' at the end of the ip.</p></li>"
                  "<li><p style=\"font-size: 1.0rem;\">To see only the temperature of room 3, simply type '/api/room/3' at the end of the ip.</p></li>"
                  "<li><p style=\"font-size: 1.0rem;\">To see only the temperature of output 1, simply type '/api/out/1' at the end of the ip.</li></p>"
                  "<li><p style=\"font-size: 1.0rem;\">To see only the temperature of output 2, simply type '/api/out/2' at the end of the ip.</li></p>"
                  "<li><p style=\"font-size: 1.0rem;\">To see only the temperature of output 3, simply type '/api/out/3' at the end of the ip.</li></p>"
                  "</div>"
                  "</ul>"
                  "</div>"
                  "</body></html>";
    request->send(200, "text/html", html);
});


       server.on("/api/room/1", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(t1).c_str());
    });
    
    server.on("/api/room/2", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(t2).c_str());
    });

    server.on("/api/room/3", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(t3).c_str());
    });

    server.on("/api/out/1", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(t4).c_str());
    });

    server.on("/api/out/2", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(t5).c_str());
    });

    server.on("/api/out/3", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(t6).c_str());
    });
  
    server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<!DOCTYPE html><html><body>{"
                      "\"model\": \"DHT11\", "
                      "\"place\": \"DC - 1 floor\","
                      "\"room1\": \"" + String(t1) + "\","
                      "\"room2\": \"" + String(t2) + "\","
                      "\"room3\": \"" + String(t3) + "\","
                      "\"out1\": \"" + String(t4) + "\","
                      "\"out2\": \"" + String(t5) + "\","
                      "\"out3\": \"" + String(t6) + "\""
                      "} </body></html>";
        request->send(200, "text/html", html);
    });
    server.begin();
}

void loop() {  
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        
        t1 = dht1.readTemperature();
        t2 = dht2.readTemperature();
        t3 = dht3.readTemperature();
        t4 = dht4.readTemperature();
        t5 = dht5.readTemperature();
        t6 = dht6.readTemperature();

        /*
        if (isnan(t1)) {
            Serial.println("Falha ao ler do sensor 1!");
        } else {
            Serial.print("Medida 1: ");
            Serial.println(t1);
        }

        if (isnan(t2)) {
            Serial.println("Falha ao ler do sensor 2!");
        } else {
            Serial.print("Medida 2: ");
            Serial.println(t2);
        }

        if (isnan(t3)) {
            Serial.println("Falha ao ler do sensor 3!");
        } else {
            Serial.print("Medida 3: ");
            Serial.println(t3);
        }

        if (isnan(t4)) {
            Serial.println("Falha ao ler do sensor 4!");
        } else {
            Serial.print("Medida 4: ");
            Serial.println(t4);
        }

        if (isnan(t5)) {
            Serial.println("Falha ao ler do sensor 5!");
        } else {
            Serial.print("Medida 5: ");
            Serial.println(t5);
        }

        if (isnan(t6)) {
            Serial.println("Falha ao ler do sensor 6!");
        } else {
            Serial.print("Medida 6: ");
            Serial.println(t6);
        }*/
    }
}
