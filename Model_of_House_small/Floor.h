class Floor {
  public:
    Floor(int lightPin) { // Конструктор класса
      // Инициируем нужный пин на выход
      pin = lightPin;
      pinMode(pin, OUTPUT);
      Light_OFF(); // На самом деле это не особо обязательно, т.к. переменные данного класса объявляются глобально и на тот момент Ардуинка не готова включать/выключать светодиоды
      lightStatus = OFF;
    };

    void Light_ON() {
      digitalWrite(pin, LOW);
      lightStatus = ON;
    };

    void Light_OFF() {
      digitalWrite(pin, HIGH);
      lightStatus = OFF;
    };




  private:
    SwitchStatus lightStatus;
    int pin;
    //Adafruit_MQTT_Subscribe Drive;


};