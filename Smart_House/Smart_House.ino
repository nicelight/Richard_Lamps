#include "DHT.h"

/* Добавляем библиотеку для IR */
#include "IRremote.h"

/* Добавляем библиотеку для I2C*/

#include "Wire.h"

/* Добавляем библиотеку для дисплея, на будущее)). ведь можернизация проекта должна включать и его ))*/
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"


// *************************************** Начало настроек *****************************
/*
   Алгоритм работы
    1)  Короткое нажатие на любую кнопку включает\выключает соответствующий свет
    2) Длительное нажатие на любую кнопку выключает все группы света
    3) Длительное (3 сек) одновременное нажатие на 1 и 4 кнопки включает \ отключает автоматический режим работы клапана обогрева
    о чем свидетельствует плавное включение, выключение сигнальных светодиодов
    4) нажатие на 2 и 3 кнопки на пол секунды включает \выключает клапан вручную в том случае, если авто режим работы отключен

    5) Кнопка - работает на вкл и выкл., но при длительном удержании "мастер кнопок" (из 6 в проекте),
    должны выключаться все световые приборы, в том числе управляемые Кнопкаами. Возобновление работы устройсв, включающих Кнопка
    должно происходить если вернуть его в положение выкл и снова включить (пока как-то так=) ) */


// к какому пину подключен сигнальный контакт датчика DHT
#define DHTPIN 50  /*ПЕРЕназначил пин*/
// максимальная температура на которой клапан будет отключаться в режиме авто
//важно ее указывать с точкой и двумя нулями после
#define MAXTEMPERATURE 22.00

// тип датчика температуры
#define DHTTYPE DHT22

// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// указываем, к каким пинам будут подключены кнопки
//кнопка подключается одним концом к пину ардуино, другим к Gnd
/*ПЕРЕназначил пины*/
#define BUTTON1 22            // кнопка включения лампы №1 /* Освещение 1 */
#define BUTTON2 24            // кнопка включения лампы №2 /* Освещение 2 */
#define BUTTON3 26            // кнопка включения лампы №3 /* Освещение периметра */
#define BUTTON4 28            // кнопка включения светодиодной подсветки. /* Cветодиодная лента RGBW*/

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
/*ПЕРЕназначил пины*/
#define LAMP1 23             /*Реле. Освещение 1*/
#define LAMP2 25             /*Реле. Освещение 2*/
#define LAMP3 27             /*Реле. Освещение периметра*/
#define RGBWSTRIP 29         /*Реле. Cветодиодная лента RGBW (периметр)*/

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
// светодиод подключается одной ногой (анод) к пину ардуино, второй ногой (катод) через резистор 150-1000 Ом  к Gnd

/*ПЕРЕназначил пины*/
#define LEDLAMP1 2            /* Светодиод. Освещение 1 */
#define LEDLAMP2 3            /* Светодиод. Освещение 2 */
#define LEDLAMP3 4            /* Светодиод. Освещение 3 */
#define LEDRGBWSTRIP 5        /* Светодиод. Cветодиодная лента RGBW (периметр) */

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

/*OLED*/
#define OLED_RESET 4 /* Выяснить какой пин можно использовать*/
Adafruit_SSD1306 display(OLED_RESET);

/*** IR ***/

/*назначаем Pin A7 для IR*/
IRrecv irrecv (53);
/*Создаем переменную для IR */
decode_results results;



/* Переменные для кодов IR */
/* Gозже заменим значения на данные кнопок пульта */
unsigned long IR_LAMP1 = 12345678;           /*IR: Освещение 1*/
unsigned long IR_LAMP2 = 12345678;           /*IR: Освещение 2*/
unsigned long IR_LAMP3 = 12345678;           /*IR: Освещение периметра*/
unsigned long IR_RGBWSTRIP = 12345678;       /*IR: Cветодиодная лента RGBW (периметр)*/
unsigned long IR_nightlight1 = 12345678;     /* IR: Ночник для чтения 1 */
unsigned long IR_nightlight2 = 12345678;     /* IR: Ночник для чтения 2 */
unsigned long IR_uvlight = 12345678;         /* IR: Ультрафиолетовая лампа */
unsigned long IR_discoball = 12345678;       /* IR: Диско-Шар */
unsigned long IR_strobeRB = 12345678;        /* IR: Строб синий + красный */
unsigned long IR_strobeW = 12345678;         /* IR: Строб белый 1*/
unsigned long IR_lightmusic1 = 12345678;     /* IR: Светомузыка1 */
unsigned long IR_lightmusic2 = 12345678;     /* IR: Светомузыка2 */
unsigned long IR_rgbwstrip2 = 12345678;      /* IR: Cветодиодная лента RGBW (подиум)*/
unsigned long IR_rezerv = 12345678;          /* IR: Резерв*/

DHT dht(DHTPIN, DHTTYPE);

bool statelamp1 = 0;                       /* Состояние: Освещение 1*/
bool statelamp2 = 0;                       /* Состояние: Освещение 2*/
bool statelamp3 = 0;                       /* Состояние: Освещение периметра*/
bool statergbwstrip = 0;                   /* Состояние: Cветодиодная лента RGBW (периметр)*/
bool stateklapan = 0;                      // состояние клапан открыто \ закрыто

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

bool klapanmode = 0;                      // состояние режима работы клапана. 0 - ручной режим, 1 - авто режим от датчика температуры



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
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); /*проверить адрес дисплея I2C*/

  /* запускаем IR*/
  irrecv.enableIRIn();

  pinMode(BUTTON1, INPUT_PULLUP);            /* Копка/Вход с подтяжкой: Освещение 1*/
  pinMode(BUTTON2, INPUT_PULLUP);            /* Копка/Вход с подтяжкой: Освещение 2*/
  pinMode(BUTTON3, INPUT_PULLUP);            /* Копка/Вход с подтяжкой: Освещение периметра*/
  pinMode(BUTTON4, INPUT_PULLUP);            /* Копка/Вход с подтяжкой: Cветодиодная лента RGBW (периметр)*/

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

  /*Реле*/
  pinMode(LAMP1, OUTPUT);                    /* Реле: Освещение 1 */
  pinMode(LAMP2, OUTPUT);                    /* Реле: Освещение 2 */
  pinMode(LAMP3, OUTPUT);                    /* Реле: Освещение 3 */
  pinMode(RGBWSTRIP, OUTPUT);                /* Реле: Cветодиодная лента RGBW (подиум) */

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
 
  /* Светодиоды */
  pinMode(LEDLAMP1, OUTPUT);                 /* Светодиод. Освещение 1 */
  pinMode(LEDLAMP2, OUTPUT);                 /* Светодиод. Освещение 2 */
  pinMode(LEDLAMP3, OUTPUT);                 /* Светодиод. Освещение 3 */
  pinMode(LEDRGBWSTRIP, OUTPUT);             /* Светодиод. Cветодиодная лента RGBW (периметр) */

  pinMode(LEDuvlight, OUTPUT);               /* Светодиод. Ультрафиолетовая лампа */
  pinMode(LEDdiscoball, OUTPUT);             /* Светодиод. Диско-Шар */
  pinMode(LEDstrobeRB, OUTPUT);              /* Светодиод. Строб синий + красный */
  pinMode(LEDstrobeW, OUTPUT);               /* Светодиод. Строб белый 1*/
  pinMode(LEDlightmusic1, OUTPUT);           /* Светодиод. Светомузыка1 */
  pinMode(LEDlightmusic2, OUTPUT);           /* Светодиод. Светомузыка2 */
  pinMode(LEDrgbwstrip2, OUTPUT);            /* Светодиод. Cветодиодная лента RGBW (подиум)*/
  pinMode(LEDrezerv, OUTPUT);                /* Светодиод. Резерв*/


  /* Новый светодиод */
  pinMode(LEDKLAPAN, OUTPUT);

  /*Клапан*/
  pinMode(KLAPAN, OUTPUT);

  digitalWrite(KLAPAN , RELAY_OFF);
  makelamps(); // включим лампы и подсветку как надо

/* I2C*/
Wire.begin();
  
}//setup

void loop() {

  ms = millis();
  autoButfunc(); // опрос кнопок и работа света
  autoKlapanfunc(); // работа клапана
  IRfunc(); /* Добавил функцию для IR*/

}//loop
