void initial_wifi();

void initial() {
  // Подключаем консоль
  Serial.begin(115200); delay(10);

  // Подключаемся к Wi-Fi
  initial_wifi();

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Light_Floor_1_Drive);
  mqtt.subscribe(&Light_Floor_2_Drive);

};

void initial_wifi() {
  Serial.println(); Serial.println("------------------------"); Serial.println();
  Serial.print("Connecting to "); Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
};