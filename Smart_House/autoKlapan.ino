void autoKlapanfunc() {
  switch (autoKlapan) {
    case 0:
      // init
      klapanMs = ms;
      autoKlapan = 5; //GO
      break;

    // выбор режима
    case 5:
      if (klapanmode) autoKlapan = 10; //GO в авторежим
      else autoKlapan = 30; // GO в ручной режим
      klapanMs = ms - 5000; // первый опрос - моментальный
      break;

    //автоматический режим
    case 10:
      if ((ms - klapanMs) > 5000) {
        klapanMs = ms;
        float t = dht.readTemperature();
        int h     = (int)dht.readHumidity();
        // проверка чтения датчика
        if (( h == ERROR_VALUE ) || (isnan(t))) {
          Serial.println("bad DHT");
          delay(2000);
          return;
        }// if error
#ifdef DEBUG
        Serial.print(" max t is ");
        Serial.print(MAXTEMPERATURE);
        Serial.print(" *C. Now ");
        Serial.print(t);
        Serial.print(" *C. Humidity ");
        Serial.print(h);
        Serial.println(" %");
#endif
        if (t < (MAXTEMPERATURE - DELTATEMPERATURE)) {
          digitalWrite(KLAPAN, RELAY_ON);
          ledKlapan = 10; // отображение  режима работы клапана
#ifdef DEBUG
          Serial.println(" *C. KLAPAN auto on");
#endif
        }// if low t
        else if (t >= MAXTEMPERATURE) {
          digitalWrite(KLAPAN, RELAY_OFF);
          ledKlapan = 20; // отображение  режима работы клапана
#ifdef DEBUG
          Serial.println(" *C. KLAPAN auto off ");
#endif
        }// if high t
      }//if ms
      break;

    // ручной режим
    case 30:
      if (stateklapan) {
        digitalWrite(KLAPAN, RELAY_ON);
        ledKlapan = 31;
      } else {
        digitalWrite(KLAPAN, RELAY_OFF);
        ledKlapan = 30;
      }
      break;

  }//switch(autoKlapan){
}//void autoKlapanfunc()
