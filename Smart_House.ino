
/*
   Алгоритм работы
    1)  Короткое нажатие на любую кнопку включает\выключает соответствующий свет
    2) Длительное нажатие на любую кнопку выключает все группы света
    3) Длительное (3 сек) одновременное нажатие на 1 и 4 кнопки включает \ отключает автоматический режим работы клапана обогрева
    о чем свидетельствует плавное включение, выключение сигнальных светодиодов
    4) нажатие на 2 и 3 кнопки на пол секунды включает \выключает клапан вручную в том случае, если авто режим работы отключен

Добавлено:
- еще 10 кнопок
- еще 10 реле
- светодиоды для состояния кнопок/реле№7-14 (для5-6 они не нужны)
- светодиод отражающий работу клапана (если он включен, то светодиод горит, выключен - наоборот) 
- все кнопки работают в таком же режиме как предыдущие. Нажал- вкл, еще раз нажал - выкл.
- Длительное нажатие выключает все (с 1 по 14).
- нажатие кнопки 5 или 6 включает каждая свое реле, но при включении кнопки 3, она включает исразу группу: 
LAMP3+nightlight1+nightlight1 (и выключает соответственно).
- никаких дополнительных меню на новых кнопках нет.

Из того что предстоит исправить в версии v2.0:

- Ошибки)))))
- Добавить включение света Lamp1+Lamp2+Lamp3+RGBWSTRIP+nightlight1+nightlight2 по длительному нажатию. (это предполагалось изначально, 
но что-то не работает. Сейчас можно подкорректировать.
- Есть какой-то невыявленный до конца мной баг - после включения в ручном режиме клапана, выключается он таким же образом с непонятным мне алгоритмом. 
Приходится по несколько раз включать его, С какого-то N-го раза это происходит.

Я в Пн планирую перенести проект на мегу (хоть какой-нибудь рабочий)) и установить мегу на свое законное место. Надеюсь светового дня мне хватит))


*/
#include "DHT.h"

// к какому пину подключен сигнальный контакт датчика DHT
#define DHTPIN 50
// максимальная температура на которой клапан будет отключаться в режиме авто
//важно ее указывать с точкой и двумя нулями после
#define MAXTEMPERATURE 22.00

// тип датчика температуры, раскомментировать нужную из трех строк
#define DHTTYPE DHT22
//#define DHTTYPE DHT22
//#define DHTTYPE DHT21

// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// указываем, к каким пинам будут подключены кнопки
//кнопка подключается одним концом к пину ардуино, другим к Gnd
#define BUTTON1 22 // кнопка включения лампы №1
#define BUTTON2 24 // кнопка включения лампы №2
#define BUTTON3 26 // кнопка включения лампы №3
#define BUTTON4 28 // кнопка включения светодиодной подсветки

/*Добавляем новые кнопки и Кнопкаы*/
#define BUTTON5 30            /* Кнопка: Ночник для чтения 1 */
#define BUTTON6 32            /* Кнопка: Ночник для чтения 2 */
#define BUTTON7 34            /* Кнопка: Ультрафиолетовая лампа */
#define BUTTON8 36            /* Кнопка: Диско-Шар */
#define BUTTON9 38            /* Кнопка: Строб синий + красный */
#define BUTTON10 40           /* Кнопка: Строб белый */
#define BUTTON11 42           /* Кнопка: Светомузыка1 */
#define BUTTON12 44           /* Кнопка: Светомузыка2 */
#define BUTTON13 46           /* Кнопка: Cветодиодная лента RGBW (кровать) */
#define BUTTON14 48           /* Кнопка: Резерв */

// в зависимости от типа кнопки, если это обычная механическая кнопка,
//подключенная к GND, то устанавливаем ACTIVE 0
// если электронная кнопка, возможно ACTIVE 1
#define ACTIVE 0

//указываем к каким пинам будут подключены реле для ламп
#define LAMP1 23
#define LAMP2 25
#define LAMP3 27
#define RGBWSTRIP 29

/* Новые реле:*/
#define nightlight1 31       /* Реле. Ночник для чтения 1 */
#define nightlight2 33       /* Реле. Ночник для чтения 2 */
#define uvlight 35           /* Реле. Ультрафиолетовая лампа */
#define discoball 37         /* Реле. Диско-Шар */
#define strobeRB 39          /* Реле. Строб синий + красный */
#define strobeW 41           /* Реле. Строб белый 1*/
#define lightmusic1 43       /* Реле. Светомузыка1 */
#define lightmusic2 45       /* Реле. Светомузыка2 */
#define rgbwstrip2 47        /* Реле. Cветодиодная лента RGBW (подиум)*/
#define rezerv 49            /* Реле. Резерв*/

// тип управления реле
// если включается нулем, оставить как есть, если включается единицей, поменять 1 и 0 местами
// чтобы убедиться что эти параметры установлены правильно, при первом включении после перепрошивки
// все лампы должны быть выключены, а все подсветки включателей - светиться
#define RELAY_ON 0
#define RELAY_OFF 1

// пин клапана воды
#define KLAPAN 51

//к каким пинам будут подключены светодиоды подсветки включаетелей
// к плате ардуино нано допустимо подключать только к пинам 3, 5, 6, 9, 10 и 11
// светодиод подключается одной ногой (анод) к пину ардуино, второй ногой (катод) через резистор 150-1000 Ом  к Gnd
#define LEDLAMP1 2
#define LEDLAMP2 3
#define LEDLAMP3 4
#define LEDRGBWSTRIP 5

/*Новыне светодиоды*/
#define LEDuvlight 6          /* Светодиод. Ультрафиолетовая лампа */
#define LEDdiscoball 7        /* Светодиод. Диско-Шар */
#define LEDstrobeRB 8         /* Светодиод. Строб синий + красный */
#define LEDstrobeW 9          /* Светодиод. Строб белый 1*/
#define LEDlightmusic1 10     /* Светодиод. Светомузыка1 */
#define LEDlightmusic2 11     /* Светодиод. Светомузыка2 */
#define LEDrgbwstrip2 12      /* Светодиод. Cветодиодная лента RGBW (подиум)*/
#define LEDrezerv 13          /* Светодиод. Резерв*/

/* Новые светодиод (состояния)*/
#define LEDKLAPAN 52          /* Светодиод: Клапан*/

// для вывода в консоль отладочных сообщений расскомментировать
//#define DEBUG
// *************************************** конец настроек *****************************

DHT dht(DHTPIN, DHTTYPE);

bool statelamp1 = 0;
bool statelamp2 = 0;
bool statelamp3 = 0;
bool statergbwstrip = 0;
bool stateklapan = 0; // состояние клапан открыто \ закрыто
bool klapanmode = 0; // состояние режима работы клапана. 0 - ручной режим, 1 - авто режим от датчика температуры

/*Добавляем состояния для реле */
bool statenightlight1 = 0;                 /* Состояние: Ночник для чтения 1 */
bool statenightlight2 = 0;                 /* Состояние: Ночник для чтения 2 */
bool stateuvlight = 0;                     /* Состояние: Ультрафиолетовая лампа */
bool statediscoball = 0;                   /* Состояние: Диско-Шар */
bool statestrobeRB = 0;                    /* Состояние: Строб синий + красный */
bool statestrobeW = 0;                     /* Состояние: Строб белый 1*/
bool statelightmusic1 = 0;                 /* Состояние: Светомузыка1 */
bool statelightmusic2 = 0;                 /* Состояние: Светомузыка2 */
bool statergbwstrip2 = 0;                  /* Состояние: Cветодиодная лента RGBW (подиум)*/
bool staterezerv = 0;                      /* Состояние: Резерв*/


byte autoBut = 0; // автомат обработки всех 4  кнопок
byte autoButprev  = 0; // для отладки
unsigned long ms = 0, butMs = 0;
// счетчик верных нажатий ( отпусканий) той или иной кнопки
byte butcount = 0;

byte autoKlapan = 0; // автомат обработки клапона. управляется из автомата кнопок
unsigned long klapanMs = 0;
void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  /* Новые Кнопки: */
  pinMode (BUTTON5, INPUT_PULLUP);           /* Копка: Вход - Ночник для чтения 1 */
  pinMode (BUTTON6, INPUT_PULLUP);           /* Копка: Вход - Ночник для чтения 2 */
  pinMode (BUTTON7, INPUT_PULLUP);           /* Копка/Вход с подтяжкой: Вход - Зона над подиумом 1 */
  pinMode (BUTTON8, INPUT_PULLUP);           /* Копка/Вход с подтяжкой: Вход - Зона над подиумом 2 */
  pinMode (BUTTON9, INPUT_PULLUP);           /* Копка/Вход с подтяжкой: Вход - Зона над подиумом 3 */
  pinMode (BUTTON10, INPUT_PULLUP);          /* Копка/Вход с подтяжкой: Вход - Зона над подиумом 4 */
  pinMode (BUTTON11, INPUT_PULLUP);          /* Копка/Вход с подтяжкой: Вход - Зона над подиумом */
  pinMode (BUTTON12, INPUT_PULLUP);          /* Копка/Вход с подтяжкой: Вход - Ультрафиолетовая лампа */
  pinMode (BUTTON13, INPUT_PULLUP);          /* Копка/Вход с подтяжкой: Вход - Светомузыка */
  pinMode (BUTTON14, INPUT_PULLUP);          /* Копка/Вход с подтяжкой: Вход - Cветодиодная лента RGBW (подиум) */

  //реле
  pinMode(LAMP1, OUTPUT);
  pinMode(LAMP2, OUTPUT);
  pinMode(LAMP3, OUTPUT);
  pinMode(RGBWSTRIP, OUTPUT);
  /*  Новые Реле:*/
  pinMode(nightlight1, OUTPUT);              /* Реле: Ночник для чтения 1 */
  pinMode(nightlight2 , OUTPUT);             /* Реле: Ночник для чтения 2 */
  pinMode(uvlight, OUTPUT);                  /* Реле: Ультрафиолетовая лампа */
  pinMode(discoball, OUTPUT);                /* Реле: Диско-Шар */
  pinMode(strobeRB, OUTPUT);                 /* Реле: Строб синий + красный */
  pinMode(strobeW, OUTPUT);                  /* Реле: Строб белый 1*/
  pinMode(lightmusic1, OUTPUT);              /* Реле: Светомузыка1 */
  pinMode(lightmusic2, OUTPUT);              /* Реле: Светомузыка2 */
  pinMode(rgbwstrip2 , OUTPUT);              /* Реле: Cветодиодная лента RGBW (подиум)*/
  pinMode(rezerv, OUTPUT);                   /* Реле: Резерв*/
  //светодиоды
  pinMode(LEDLAMP1, OUTPUT);
  pinMode(LEDLAMP2, OUTPUT);
  pinMode(LEDLAMP3, OUTPUT);
  pinMode(LEDRGBWSTRIP, OUTPUT);
  //новые светодиоды
  pinMode(LEDuvlight, OUTPUT);               /* Светодиод. Ультрафиолетовая лампа */
  pinMode(LEDdiscoball, OUTPUT);             /* Светодиод. Диско-Шар */
  pinMode(LEDstrobeRB, OUTPUT);              /* Светодиод. Строб синий + красный */
  pinMode(LEDstrobeW, OUTPUT);               /* Светодиод. Строб белый 1*/
  pinMode(LEDlightmusic1, OUTPUT);           /* Светодиод. Светомузыка1 */
  pinMode(LEDlightmusic2, OUTPUT);           /* Светодиод. Светомузыка2 */
  pinMode(LEDrgbwstrip2, OUTPUT);            /* Светодиод. Cветодиодная лента RGBW (подиум)*/
  pinMode(LEDrezerv, OUTPUT);                /* Светодиод. Резерв*/
  pinMode(LEDKLAPAN, OUTPUT);                /* Светодиод. Клапан*/

  pinMode(KLAPAN, OUTPUT);
  digitalWrite(KLAPAN , RELAY_OFF);
  makelamps(); // включим лампы и подсветку как надо
}//setup

void loop() {

  ms = millis();
  autoButfunc(); // опрос кнопок и работа света
  autoKlapanfunc(); // работа клапана

}//loop