void ledKlapanfunc() {
  /// светодиод индикация режима работы и состояния клапана
  switch (ledKlapan) {
    case 0:
      ledKlapanMs = ms;
      ledKlapan = 10; // GO
      break;
    // 10-12 режим авто, когда клапан включен ( тепло поступает)
    case 10:
      if ((ms - ledKlapanMs) > 100) {
        ledKlapanMs = ms;
        digitalWrite(LEDKLAPAN, 1);
        ledKlapan = 12; // GO
      }//if ms
      break;
    case 12:
      if ((ms - ledKlapanMs) > 900) {
        ledKlapanMs = ms;
        digitalWrite(LEDKLAPAN, 0);
        ledKlapan = 10; // GO
      }//if ms
      break;

    // 20-22 режим авто, когда клапан ВЫключен ( тепло не поступает)
    case 20:
      if ((ms - ledKlapanMs) > 100) {
        ledKlapanMs = ms;
        digitalWrite(LEDKLAPAN, 0);
        ledKlapan = 22; // GO
      }//if ms
      break;
    case 22:
      if ((ms - ledKlapanMs) > 900) {
        ledKlapanMs = ms;
        digitalWrite(LEDKLAPAN, 1);
        ledKlapan = 20; // GO
      }//if ms
      break;

    // режим, клапан вЫключен вручную
    case 30:
      ledKlapanMs = ms;
      digitalWrite(LEDKLAPAN, 0);
      break;
    // режим, клапан Включен вручную
    case 31:
      ledKlapanMs = ms;
      digitalWrite(LEDKLAPAN, 1);
      break;
  }//switch(ledKlapan)
}//void ledKlapanfunc()
