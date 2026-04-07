# IOT-ABA

Projet IoT embarqué avec ESP32 pour la surveillance environnementale via capteur BME280.

Le firmware lit trois variables:
- temperature
- humidite
- pression

Ces mesures sont sérialisées au format JSON puis publiées sur un broker MQTT public.

## Materiel

- ESP32 (board: esp32dev)
- Capteur BME280 (I2C, adresse 0x76 ou 0x77)
- Cables Dupont

## Architecture logicielle

- Initialisation capteur dans la fonction demarrerLeCapteur
- Connexion Wi-Fi dans la fonction demarrerInternet
- Connexion broker MQTT dans la fonction connecterAuBroker
- Publication JSON dans la fonction envoyerTroisVariables
- Boucle de supervision dans loop avec reconnexion Wi-Fi et MQTT

Le topic de publication est:

- capteur/bme280

Exemple de payload JSON:

```json
{
	"temperature": 24.3,
	"humidite": 52.1,
	"pression": 1009.6
}
```

## Configuration PlatformIO

Le projet utilise:

- platform: espressif32
- board: esp32dev
- framework: arduino
- monitor_speed: 115200

Dependances:

- knolleary/PubSubClient
- adafruit/Adafruit BME280 Library
- bblanchon/ArduinoJson

Voir la configuration dans [platformio.ini](platformio.ini).

## Configuration reseau et MQTT

Actuellement, les informations suivantes sont codées en dur dans [src/main.cpp](src/main.cpp):

- SSID Wi-Fi
- mot de passe Wi-Fi
- hote MQTT
- port MQTT
- client ID MQTT

Pour la production, il est recommande de sortir ces valeurs du code source.

## Build, Upload, Monitor

Depuis le dossier du projet:

```bash
platformio run
platformio run --target upload --upload-port COM3
platformio device monitor --baud 115200
```

Ou directement via les taches PlatformIO de VS Code.

## Structure du projet

- [src/main.cpp](src/main.cpp): logique firmware principale
- [platformio.ini](platformio.ini): configuration environnement et dependances
- [include](include): headers utilisateur (optionnel)
- [lib](lib): bibliotheques locales (optionnel)
- [test](test): tests unitaires (optionnel)

## Etat actuel

- Lecture BME280 fonctionnelle
- Publication MQTT JSON active
- Reconnexion MQTT en cas de deconnexion
- Temporisation actuelle avec delay(1000) a optimiser en version non bloquante pour un usage industriel