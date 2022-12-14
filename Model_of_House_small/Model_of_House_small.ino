/***************************************************
  скетч для управления макетом домика через MQTT брокера
  Макет содержит два управляемых светильника

  Автор: Матыжонок Виктор
 ****************************************************/

/************ Подключаем внешние библиотеки ******************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************ Подключаем файл с паролями доступа к Wi-Fi и брокеру MQTT ******************/
#include "arduino_secrets.h"

/************ Подключаем мои классы ******************/
enum SwitchStatus {OFF, ON};

#include "Floor.h"

/************ Global State ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Подписки ***************************************/

// Подписываемся на каналы управления осведением (для получения команд на управление светом)
Adafruit_MQTT_Subscribe Light_Floor_1_Drive = Adafruit_MQTT_Subscribe(&mqtt, "LittleHouse_small/Floor_1/Light/Drive");
Adafruit_MQTT_Subscribe Light_Floor_2_Drive = Adafruit_MQTT_Subscribe(&mqtt, "LittleHouse_small/Floor_2/Light/Drive");

// Подписываемся на публикацию текущего статуса освещения
Adafruit_MQTT_Publish Light_Floor_1_Status = Adafruit_MQTT_Publish(&mqtt, "LittleHouse_small/Floor_1/Light/Status");
Adafruit_MQTT_Publish Light_Floor_2_Status = Adafruit_MQTT_Publish(&mqtt, "LittleHouse_small/Floor_2/Light/Status");


// Определяем пины, на которых будет управление
#define FLOOR_1_PIN D1
#define FLOOR_2_PIN D2

// Типы данных


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void initial();

Floor floor_1(FLOOR_1_PIN);
Floor floor_2(FLOOR_2_PIN);

String command = "";

void setup() {
  initial();
}


void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
   /* if (subscription == &Light_Floor_1_Drive) {
      Serial.print(F("Got: "));
      state = (char *)Light_Floor_1_Drive.lastread;
      Serial.println((char *)Light_Floor_1_Drive.lastread);
    }*/
    command = "";
    if (subscription == &Light_Floor_1_Drive) {
      command = (char *)Light_Floor_1_Drive.lastread;
      if (command == "ON") {
        floor_1.Light_ON();
        Light_Floor_1_Status.publish("ON");
      };
      if (command == "OFF") {
        floor_1.Light_OFF();
        Light_Floor_1_Status.publish("OFF");        
      };
    };

    if (subscription == &Light_Floor_2_Drive) {
      command = (char *)Light_Floor_2_Drive.lastread;
      if (command == "ON") {
        floor_2.Light_ON();
        Light_Floor_2_Status.publish("ON");
      };
      if (command == "OFF") {
        floor_2.Light_OFF();
        Light_Floor_2_Status.publish("OFF");
      };
    };
  }

  // Now we can publish stuff!
 /* Serial.print(F("\nSending Light_Floor_1_Status val "));
  Serial.print("...");
  if (! Light_Floor_1_Status.publish("ON")) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }*/

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
