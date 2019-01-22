#include <SoftwareSerial.h>

/*
   Алгоритм работы
    1)  Короткое нажатие на любую кнопку включает\выключает соответствующий свет
    2) Длительное нажатие на любую кнопку выключает все группы света
    3) Длительное (3 сек) одновременное нажатие на 1 и 4 кнопки включает \ отключает автоматический режим работы клапана обогрева
    о чем свидетельствует плавное включение, выключение сигнальных светодиодов
    4) нажатие на 2 и 3 кнопки на пол секунды включает \выключает клапан вручную в том случае, если авто режим работы отключен

Добавлено:

- светодиод отражающий работу клапана (если он включен, то светодиод горит, выключен - наоборот) 
- нажатие кнопки 5 или 6 включает каждая свое реле, но при включении кнопки 3, она включает и сразу группу: 
LAMP3+nightlight1+nightlight1 (и выключает соответственно).

*/
#include "DHT.h"

// к какому пину подключен сигнальный контакт датчика DHT
#define DHTPIN 50
// максимальная температура на которой клапан будет отключаться в режиме авто
//важно ее указывать с точкой и двумя нулями после
#define MAXTEMPERATURE 22.00
// на сколько градусов ниже должно стать, чтобы клапан снова открылся
// если используется датчик dht11 ( c плохой чувствительностью) не рекомендуется ставить дельту ниже 2 градусов
#define DELTATEMPERATURE 1.00 //Ринат: у меня dht22 дельты в 1 мне хватит? 
// тип датчика температуры, раскомментировать нужную из трех строк
#define DHTTYPE DHT22
//#define DHTTYPE DHT22
//#define DHTTYPE DHT21
#define ERROR_VALUE 2147483647

// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// указываем, к каким пинам будут подключены кнопки
//кнопка подключается одним концом к пину ардуино, другим к Gnd
#define BUTTON1 22 // кнопка включения лампы №1
#define BUTTON2 24 // кнопка включения лампы №2
#define BUTTON3 26 // кнопка включения лампы №3
#define BUTTON4 28 // кнопка включения светодиодной подсветки

/*Добавляем новые кнопки*/
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

/* Новые реле

Сергей:"здесь и далее в скетче необхходимо исправить эти дефайны"
чтобы были только заглавные 
типа 
#define NIGHTLIGHT1 31
а ниже 
pinMode(NIGHTLIGHT1, OUTPUT);

Ринат: Дефайны - исправлено во всем скетче

*/
#define NIGHTLIGHT1 31       /* Реле. Ночник для чтения 1 */
#define NIGHTLIGHT2 33       /* Реле. Ночник для чтения 2 */
#define UVLIGHT 35           /* Реле. Ультрафиолетовая лампа */
#define DISCOBALL 37         /* Реле. Диско-Шар */
#define STROBERB 39          /* Реле. Строб синий + красный */
#define STROBEW 41           /* Реле. Строб белый 1*/
#define LIGHTMUSIC1 43       /* Реле. Светомузыка1 */
#define LIGHTMUSIC2 45       /* Реле. Светомузыка2 */
#define RGBWSTRIP2 47        /* Реле. Cветодиодная лента RGBW (подиум)*/
#define REZERV 49            /* Реле. Резерв*/

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

/*Новые светодиоды*/
#define LEDUVLIGHT 6          /* Светодиод. Ультрафиолетовая лампа */
#define LEDDISCOBALL 7        /* Светодиод. Диско-Шар */
#define LEDSTROBERB 8         /* Светодиод. Строб синий + красный */
#define LEDSTROBEW 9          /* Светодиод. Строб белый 1*/
#define LEDLIGHTMUSIC1 10     /* Светодиод. Светомузыка1 */
#define LEDLIGHTMUSIC2 11     /* Светодиод. Светомузыка2 */
#define LEDRGBWSTRIP2 12      /* Светодиод. Cветодиодная лента RGBW (подиум)*/
#define LEDREZERV 13          /* Светодиод. Резерв*/

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
bool stateklapan = 0; // состояние клапана в ручном режиме: открыто \ закрыто
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

byte autoKlapan = 0; // автомат обработки клапана. управляется из автомата кнопок
unsigned long klapanMs = 0;
byte ledKlapan = 0; // автомат отрисовки состояния клапана
unsigned long ledKlapanMs = 0;

void setup() {
  //Serial.begin(115200); Ринат - помню Вы говорили, что серийник сильно тормозит программу, пока убрал (до начала отладки)
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
  pinMode(NIGHTLIGHT1, OUTPUT);              /* Реле: Ночник для чтения 1 */
  pinMode(NIGHTLIGHT2, OUTPUT);              /* Реле: Ночник для чтения 2 */
  pinMode(UVLIGHT, OUTPUT);                  /* Реле: Ультрафиолетовая лампа */
  pinMode(DISCOBALL, OUTPUT);                /* Реле: Диско-Шар */
  pinMode(STROBERB, OUTPUT);                 /* Реле: Строб синий + красный */
  pinMode(STROBEW, OUTPUT);                  /* Реле: Строб белый 1*/
  pinMode(LIGHTMUSIC1, OUTPUT);              /* Реле: Светомузыка1 */
  pinMode(LIGHTMUSIC2, OUTPUT);              /* Реле: Светомузыка2 */
  pinMode(RGBWSTRIP2 , OUTPUT);              /* Реле: Cветодиодная лента RGBW (подиум)*/
  pinMode(REZERV, OUTPUT);                   /* Реле: Резерв*/
  //светодиоды
  pinMode(LEDLAMP1, OUTPUT);
  pinMode(LEDLAMP2, OUTPUT);
  pinMode(LEDLAMP3, OUTPUT);
  pinMode(LEDRGBWSTRIP, OUTPUT);
  //новые светодиоды
  pinMode(LEDUVLIGHT, OUTPUT);               /* Светодиод. Ультрафиолетовая лампа */
  pinMode(LEDDISCOBALL, OUTPUT);             /* Светодиод. Диско-Шар */
  pinMode(LEDSTROBERB, OUTPUT);              /* Светодиод. Строб синий + красный */
  pinMode(LEDSTROBEW, OUTPUT);               /* Светодиод. Строб белый 1*/
  pinMode(LEDLIGHTMUSIC1, OUTPUT);           /* Светодиод. Светомузыка1 */
  pinMode(LEDLIGHTMUSIC2, OUTPUT);           /* Светодиод. Светомузыка2 */
  pinMode(LEDRGBWSTRIP2, OUTPUT);            /* Светодиод. Cветодиодная лента RGBW (подиум)*/
  pinMode(LEDREZERV, OUTPUT);                /* Светодиод. Резерв*/
  pinMode(LEDKLAPAN, OUTPUT);                /* Светодиод. Клапан*/

  pinMode(KLAPAN, OUTPUT);
  digitalWrite(KLAPAN , RELAY_OFF);
  makelamps(); // включим лампы и подсветку как надо
}//setup

void loop() {

  ms = millis();
  autoButfunc(); // опрос кнопок и работа света
  autoKlapanfunc(); // работа клапана
  ledKlapanfunc(); // отрисовка состояния клапана

}//loop
