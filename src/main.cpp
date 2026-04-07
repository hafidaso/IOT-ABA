#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

Adafruit_BME280 capteur;
WiFiClient reseau;
PubSubClient mqtt(reseau);

const char* ssid = "IDS SALE";
const char* password = "IDS@2023";
const char* mqttHost = "broker.hivemq.com";
const uint16_t mqttPort = 1883;
const char* mqttClientId = "StationMeteo_ESP32";
const char* mqttTopic = "capteur/bme280";

void demarrerLeCapteur() {
    if (!capteur.begin(0x76) && !capteur.begin(0x77)) {
        Serial.println("Erreur : Capteur BME280 introuvable !");
    }
}

void connecterAuBroker() {
    while (!mqtt.connected()) {
        Serial.print("Connexion MQTT...");
        if (mqtt.connect(mqttClientId)) {
            Serial.println("connecte");
        } else {
            Serial.print("echec, rc=");
            Serial.print(mqtt.state());
            Serial.println(" -> nouvelle tentative dans 2s");
            delay(2000);
        }
    }
}

void demarrerInternet() {
    Serial.print("Connexion au Wi-Fi ");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnecte au Wi-Fi !");
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());

    mqtt.setServer(mqttHost, mqttPort);
    connecterAuBroker();
}

void envoyerTroisVariables() {
    mqtt.loop();

    float t = capteur.readTemperature();
    float h = capteur.readHumidity();
    float p = capteur.readPressure() / 100.0F;

    StaticJsonDocument<200> doc;
    doc["temperature"] = t;
    doc["humidite"] = h;
    doc["pression"] = p;

    char buffer[256];
    serializeJson(doc, buffer);

    mqtt.publish(mqttTopic, buffer);
    Serial.print("JSON envoye : ");
    Serial.println(buffer);
}

void direTemperature() {
    Serial.print("Temperature : ");
    Serial.println(capteur.readTemperature());
}

void direHumidite() {
    Serial.print("Humidite : ");
    Serial.println(capteur.readHumidity());
}

void direPression() {
    Serial.print("Pression : ");
    Serial.println(capteur.readPressure() / 100.0F);
}

void faireUnePause() {
    Serial.println("---");
    delay(1000);
}

void setup() {
    Serial.begin(115200);
    demarrerLeCapteur();
    demarrerInternet();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        demarrerInternet();
    }

    if (!mqtt.connected()) {
        connecterAuBroker();
    }

    direTemperature();
    direHumidite();
    direPression();
    envoyerTroisVariables();
    faireUnePause();
}