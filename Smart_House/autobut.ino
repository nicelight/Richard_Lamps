void autoButfunc() {
  /* algorithm :
      3- если нажата какая либо кнопка
      5- нажата 1. Если еще и 3 то 60- (действия по удержанию двух кнопок)
      10- если нажата 100*15 раз кнопка 1 то 50- (отключение всего света) сюда попадают из любой из 4х кнопок
      аналогично для других кнопок, опишу альтернативные состояния
      кнопка 1 \2 \3 \4
      состояния 5- \6- \7- \8-
      состояния 10- \20- \30- \40-
      далее
      50 - отключение всего света и на 99-
      60- если нажата 1 и 4, 5 раз по 100 мс, инвертим режим работы клапана(авто, ручной) и идем на 99-
      80- если нажата 2 и 3, 2 раза по 100 мс, то если режим клапана ручной, инвертим состояние работы клапана и на 99-
      99- ожидание отпускания всех кнопок , сюда попадаем из кучи состояний , типа завершающее состояние и идем на новый цикл
  */
  switch (autoBut) {
    case 0:
      // инициализация автомата
      butMs = ms;
      butcount = 0;
      autoBut = 3; // GO
      break;
    case 3:
      if ((ms - butMs) > 10) {
        butMs = ms;
        if (digitalRead(BUTTON1) == ACTIVE) {
          Serial.println("knopka 1");
          autoBut = 5; //GO
        } else if (digitalRead(BUTTON2) == ACTIVE) {
          Serial.println("knopka 2");
          autoBut = 6; //GO
        } else if (digitalRead(BUTTON3) == ACTIVE) {
          Serial.println("knopka 3");
          autoBut = 7; //GO
        } else if (digitalRead(BUTTON4) == ACTIVE) {
          Serial.println("knopka 4");
          autoBut = 8;

          /*Новые кнопки*/
        } else if (digitalRead(BUTTON5) == ACTIVE) {
          Serial.println("knopka 5");
          autoBut = 9;
        } else if (digitalRead(BUTTON6) == ACTIVE) {
          Serial.println("knopka 6");
          autoBut = 10;
        } else if (digitalRead(BUTTON7) == !ACTIVE) { //Или если кнопка7 вкл
          Serial.println("knopka 7");
          autoBut = 11; /*тогда переходим на кейс 11 */
        } else if (digitalRead(BUTTON8) == !ACTIVE) { //Или если кнопка8 вкл
          Serial.println("knopka 8");
          autoBut = 12; /*тогда переходим на кейс 12 */
        } else if (digitalRead(BUTTON9) == !ACTIVE) { //Или если кнопка9 вкл
          Serial.println("knopka 9");
          autoBut = 13; /*тогда переходим на кейс 13 */
        } else if (digitalRead(BUTTON10) == !ACTIVE) { //Или если кнопка10 вкл
          Serial.println("knopka 10");
          autoBut = 14; /*тогда переходим на кейс 14 */
        } else if (digitalRead(BUTTON11) == !ACTIVE) { //Или если кнопка11 вкл
          Serial.println("knopka 11");autoBut = 15; /*тогда переходим на кейс 15 */
        } else if (digitalRead(BUTTON12) == !ACTIVE) { //Или если кнопка12 вкл
          Serial.println("knopka 12");
          autoBut = 16; /*тогда переходим на кейс 16 */
        } else if (digitalRead(BUTTON13) == !ACTIVE) { //Или если кнопка13 вкл
          Serial.println("knopka 13");
          autoBut = 17; /*тогда переходим на кейс 17 */
        } else if (digitalRead(BUTTON14) == !ACTIVE) { //Или если кнопка14 вкл
          Serial.println("knopka 14");
          autoBut = 18; /*тогда переходим на кейс 18 *///GO
        }//if any Button
      }//if ms
      break;
    case 5:
      // кнопка 1 нажата? Если да, то если нажата еще и кнопка 4, идем на их взаимную обработку
      // пока кнопка 1 нажата, если больше 30*50 мс, то нажатие длительное - отключаем весь свет
      // если кнопку отпустили раньше чем 30*50, нажатие короткое - меняем состояние света
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON1) == ACTIVE) {
          if (digitalRead(BUTTON4) == ACTIVE) {
            // нажаты 1 и 4, идем на их взаимную обработку
            butcount = 0;
            autoBut = 60; //GO
#ifdef DEBUG
            Serial.println("Knopka1 + Knopka4");
#endif
          }// 1 & 4
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("Dolgoe Nazhatie - Viklyuchit ves' svet");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;
            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 1 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 1 fixed");
#endif
            butcount = 0;
            statelamp1 = !statelamp1; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;
    case 6:
      // кнопка 2 нажата? Если да, то если нажата еще и кнопка 3, идем на их взаимную обработку
      // пока кнопка 2 нажата, если больше 30*50 мс, то нажатие длительное - отключаем весь свет
      // если кнопку отпустили раньше чем 30*50, нажатие короткое - меняем состояние света
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON2) == ACTIVE) {
          if (digitalRead(BUTTON3) == ACTIVE) {
            // нажаты 2 и 3, идем на их взаимную обработку
            butcount = 0;
            autoBut = 80; //GO на попытку ручного переключения клапана
#ifdef DEBUG
            Serial.println("Knopka2+Knopka3");
#endif
          }// 2 & 3
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 2 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;
            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 2 fixed");
#endif
            butcount = 0;
            statelamp2 = !statelamp2; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;
    case 7:
      // кнопка 3 нажата? Если да, то если нажата еще и кнопка 2, идем на их взаимную обработку
      // пока кнопка 2 нажата, если больше 30*50 мс, то нажатие длительное - отключаем весь свет
      // если кнопку отпустили раньше чем 30*50, нажатие короткое - меняем состояние света
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON3) == ACTIVE) {
          if (digitalRead(BUTTON2) == ACTIVE) {
            // нажаты 3 и 2, идем на их взаимную обработку
            butcount = 0;
            autoBut = 80; //GO на попытку ручного переключения клапана
#ifdef DEBUG
            Serial.println("3 & 2 but noticed");
#endif
          }// 3 & 2
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 3 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;
            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statelamp3 = !statelamp3; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;
    case 8:
      // кнопка 4 нажата? Если да, то если нажата еще и кнопка 1, идем на их взаимную обработку
      // пока кнопка 4 нажата, если больше 30*50 мс, то нажатие длительное - отключаем весь свет
      // если кнопку отпустили раньше чем 30*50, нажатие короткое - меняем состояние света
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON4) == ACTIVE) {
          if (digitalRead(BUTTON1) == ACTIVE) {
            // нажаты 4 и 1, идем на их взаимную обработку
            butcount = 0;
            autoBut = 60; //GO
#ifdef DEBUG
            Serial.println("4 & 1 but noticed");
#endif
          }// 4 & 1
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;
            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 1 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 4 fixed");
#endif
            butcount = 0;
            statergbwstrip = !statergbwstrip; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    /*НОВЫЕ КЕЙСЫ!!!*/
    /* Создаем новый кейс9. для Ночника 1 (кнопка 5). С аналогичным режимом как у кнопок*/
    case 9:
      // если кнопку отпустили раньше чем 30*50, нажатие короткое - меняем состояние света
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON5) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statenightlight1 = !statenightlight1; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    /* Создаем новый кейс10. для Ночника 2 (кнопка 6). С аналогичным режимом как у кнопок*/
    case 10:
      // если кнопку отпустили раньше чем 30*50, нажатие короткое - меняем состояние света
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON6) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statenightlight2 = !statenightlight2; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    /* Создаем новый кейс11. Для uvlight (кнопка 7). С аналогичным режимом как у кнопок*/
    case 11:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON7) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            stateuvlight = !stateuvlight; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    /* Создаем новый кейс12. Для discoball (кнопка 8). С аналогичным режимом как у кнопок*/
    case 12:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON8) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statediscoball = !statediscoball; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    /* Создаем новый кейс13. Для strobeRB (кнопка 9). С аналогичным режимом как у кнопок*/
    case 13:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON9) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statestrobeRB = !statestrobeRB; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;



    /* Создаем новый кейс14. Для strobeW (кнопка 10). С аналогичным режимом как у кнопок*/
    case 14:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON10) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statestrobeW = !statestrobeW; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    /* Создаем новый кейс15. Для lightmusic1 (кнопка 11). С аналогичным режимом как у кнопок*/
    case 15:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON11) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statelightmusic1 = !statelightmusic1; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;


    /* Создаем новый кейс16. Для lightmusic2 (кнопка 12). С аналогичным режимом как у кнопок*/
    case 16:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON12) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statelightmusic2 = !statelightmusic2; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;


    /* Создаем новый кейс17. Для rgbwstrip2 (кнопка 13). С аналогичным режимом как у кнопок*/
    case 17:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON13) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            statergbwstrip2 = !statergbwstrip2; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    /* Создаем новый кейс18. Для rezerv (кнопка 14). С аналогичным режимом как у кнопок*/
    case 18:
      if ((ms - butMs) > 30) {
        butMs = ms;
        if (digitalRead(BUTTON14) == ACTIVE) {
          if (butcount < 50) {
            butcount++;
          } else { // зафиксировано длительное удержание кнопки
            // выключаем весь свет
#ifdef DEBUG
            Serial.println("long 4 fixed");
#endif
            statelamp1 = statelamp2 = statelamp3 = statergbwstrip  = statenightlight1 = statenightlight2 = stateuvlight =
                statediscoball = statestrobeRB = statestrobeW = statelightmusic1 = statelightmusic2 = statergbwstrip2 = staterezerv = 0;

            makelamps(); //включим лампы и подсветку в новое состояние
            butcount = 0;
            autoBut = 99; // GO на ожидание отпускания кнопок
          }
        } else { //BUT 2 released
          if (butcount) {
#ifdef DEBUG
            Serial.println("short 3 fixed");
#endif
            butcount = 0;
            staterezerv = !staterezerv; // инвертируем состояние света
            makelamps(); //включим лампы и подсветку в новое состояние
          }
          autoBut = 99; // GO на ожидание отпускания кнопок
        }// no buttons pressed
      }//if ms
      break;

    case 60:
      // одновременно нажаты 1 и 4, меняем klapanmode
      if ((ms - butMs) > 100) {
        butMs = ms;
        if ((digitalRead(BUTTON1) == ACTIVE) && (digitalRead(BUTTON4) == ACTIVE)) {
          if ( butcount < 30) {
            butcount ++;
          } else { // обе кнопки удерживались 3 секунды
            butcount = 0;
#ifdef DEBUG
            Serial.println("inverting klapanmode");
#endif
            klapanmode = !klapanmode; // ИНВЕРТИРУЕМ РЕЖИМ РАБОТЫ КЛАПАНА
            autoBut = 99; //GO на ожидание опускания
          }//1 & 4 fixed
        } else { // недодержали обе кнопки
          butcount = 0;
          autoBut = 99; // GO на ожидание отпускания кнопок
        }
      }//if ms
      break;

    case 80:
      // одновременно нажаты 2 и 3 =>  Если режим управления клапаном ручной, позволяем изменить состояние клапана
      if ((ms - butMs) > 100) {
        butMs = ms;
        if ((digitalRead(BUTTON2) == ACTIVE) && (digitalRead(BUTTON3) == ACTIVE)) {
          if ( butcount < 5) {
            butcount ++;
          } else { // обе кнопки удерживались 3 секунды
            butcount = 0;
            // если режим клапана ручной, переключим состояние клапана? приходится костылить и напрямую управлять автоматом клапана чтобы красивая отрисовка была
            if (!klapanmode) {
              if (stateklapan) { // если клапан включен
                autoKlapan = 22; // ручное отключение клапана GO
#ifdef DEBUG
                Serial.println("KLAPAN OFF manually");
#endif
              } else {
                autoKlapan = 24; // ручное включение клапана GO
#ifdef DEBUG
                Serial.println("manually KLAPAN ON");
#endif
              }
            } // if klapanmode = manual
            else {
#ifdef DEBUG
              Serial.println("not allowed manually drive KLAPAN");
#endif
              autoKlapan = 26; // отрисовка запрета на изменение состояния клапана
            }
            autoBut = 99; //GO на ожидание опускания
          }//2 & 3 fixed
        } else { // недодержали обе кнопки
          butcount = 0;
          autoBut = 99; // GO на ожидание отпускания кнопок
        }
      }//if ms
      break;

    case 99:
      // ждем пока все кнопки будут отпущены
      if ((ms - butMs) > 20) {
        butMs = ms;
        if ((digitalRead(BUTTON1) != ACTIVE) && (digitalRead(BUTTON2) != ACTIVE) && (digitalRead(BUTTON3) != ACTIVE) && (digitalRead(BUTTON4) != ACTIVE) &&
            (digitalRead(BUTTON5) != ACTIVE) && (digitalRead(BUTTON6) != ACTIVE) && (digitalRead(BUTTON7) != ACTIVE) && (digitalRead(BUTTON8) != ACTIVE) &&
            (digitalRead(BUTTON9) != ACTIVE) && (digitalRead(BUTTON10) != ACTIVE) && (digitalRead(BUTTON11) != ACTIVE) && (digitalRead(BUTTON12) != ACTIVE) &&
            (digitalRead(BUTTON13) != ACTIVE) && (digitalRead(BUTTON14) != ACTIVE)) {
          if (!butcount) {
            butcount++;
          } else {
            butcount = 0;
            autoBut = 3; // GO
          }
        }//if no active buttons
      }//if ms
  }//switch(autobut)

  /*
    #ifdef DEBUG
    // отладка автомата кнопок
    if(autoButprev != autoBut){
    autoButprev  = autoBut;
    Serial.print("autoBut = ");
    Serial.println(autoBut);
    }
    #endif
  */
}//void autoButfunc()


// функция переключения состояний ламп
void makelamps() {
  // включаем лампы как надо
  if (statelamp1) {
    digitalWrite(LAMP1, RELAY_ON);
  }
  else {
    digitalWrite(LAMP1, RELAY_OFF);
  }
  if (statelamp2) {
    digitalWrite(LAMP2, RELAY_ON);
  }
  else {
    digitalWrite(LAMP2, RELAY_OFF);
  }

  if (statergbwstrip) {
    digitalWrite(RGBWSTRIP, RELAY_ON);
  }
  else {
    digitalWrite(RGBWSTRIP, RELAY_OFF);
  }

  /* Добавляем одновременное включение и выключение ночников при нажатии кнопки 3*/
  if (statelamp3) {
    digitalWrite(LAMP2, RELAY_ON);
    digitalWrite(nightlight1, RELAY_ON);
    digitalWrite(nightlight2, RELAY_ON);
  }//if
  else {
    digitalWrite(LAMP2 , RELAY_ON);
    digitalWrite(nightlight1, RELAY_OFF);
    digitalWrite(nightlight2, RELAY_OFF);
  }//else


  /* Добавим работу остальных реле, управляемые кнопками*/
  if (statenightlight1) {
    digitalWrite(nightlight1, RELAY_ON);
  }
  else {
    digitalWrite(nightlight1, RELAY_OFF);
  }
  if (statenightlight2) {
    digitalWrite(nightlight2, RELAY_ON);
  }
  else {
    digitalWrite(nightlight2, RELAY_OFF);
  }
  if (stateuvlight) {
    digitalWrite(uvlight, RELAY_ON);
  }
  else {
    digitalWrite(uvlight, RELAY_OFF);
  }
  if (statediscoball) {
    digitalWrite(discoball, RELAY_ON);
  }
  else {
    digitalWrite(discoball, RELAY_OFF);
  }
  if (statestrobeRB) {
    digitalWrite(strobeRB, RELAY_ON);
  }
  else {
    digitalWrite(strobeRB, RELAY_OFF);
  }
  if (statestrobeW) {
    digitalWrite(strobeW, RELAY_ON);
  }
  else {
    digitalWrite(strobeW, RELAY_OFF);
  }
  if (statelightmusic1) {
    digitalWrite(lightmusic1, RELAY_ON);
  }
  else {
    digitalWrite(lightmusic1, RELAY_OFF);
  }
  if (statelightmusic2) {
    digitalWrite(lightmusic2, RELAY_ON);
  }
  else {
    digitalWrite(lightmusic2, RELAY_OFF);
  }
  if (statergbwstrip2) {
    digitalWrite(rgbwstrip2, RELAY_ON);
  }
  else {
    digitalWrite(rgbwstrip2, RELAY_OFF);
  }
  if (staterezerv) {
    digitalWrite(rezerv, RELAY_ON);
  }
  else {
    digitalWrite(rezerv, RELAY_OFF);
  }

  //включаем подсветку ламп как надо
  digitalWrite(LEDLAMP1, (!statelamp1));
  digitalWrite(LEDLAMP2, (!statelamp2));
  digitalWrite(LEDLAMP3, (!statelamp3));
  digitalWrite(LEDRGBWSTRIP, (!statergbwstrip));

  /* Добавил подсветку*/
  digitalWrite(LEDuvlight, (!stateuvlight));
  digitalWrite(LEDdiscoball, (!statediscoball));
  digitalWrite(LEDstrobeRB, (!statestrobeRB));
  digitalWrite(LEDstrobeW, (!statestrobeW));
  digitalWrite(LEDlightmusic1, (!statelightmusic1));
  digitalWrite(LEDlightmusic2, (!statelightmusic2));
  digitalWrite(LEDrgbwstrip2, (!statergbwstrip2));
  digitalWrite(LEDrezerv, (!staterezerv));



}//void makelamps()

// для мигания подсветкой когда с клапаном что то делаем
void showKlapanState(byte i) {
  analogWrite(LEDLAMP1, i);
  analogWrite(LEDLAMP2, i);
  analogWrite(LEDLAMP3, i);
  analogWrite(LEDRGBWSTRIP, i);

}

