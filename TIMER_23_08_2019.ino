
#include<Wire.h>
#include "DS3231.h"
#include <LiquidCrystal.h>     // inclui a biblioteca LiquidCrystal


RTClib RTC;

// inicializa a biblioteca e declara os pinos do LCD connectados ao Arduinp
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte relogio[8] = {B00000, B01110, B10101, B10111, B10001, B01110, B00000, B00000};


int const b1 = 6, b2 = 7, b3 = 9, led = 13;                                                         // declara os pinos dos botões 1,2 e 3 e do item a ser ligado ou desligado (neste caso, um LED funcionará como sinalizador)
int  segundo = 0, minuto = 0, hora = 0, dez_hora, uni_hora, dez_minuto, uni_minuto;                // variáveis que irão marcar o tempo
int hora_on = 0, minuto_on = 0, dez_hora_on, uni_hora_on, dez_minuto_on, uni_minuto_on;           // variáveis para marcar o tempo de configuração para ligar um item (TIMER ON)
int hora_off = 0, minuto_off = 0, dez_hora_off, uni_hora_off, dez_minuto_off, uni_minuto_off;    // variáveis para marcar o tempo de configuração para desligar um item (TIMER OFF)
int dia, dia_timer = 1;
int dia_rtc, mes_rtc, ano_rtc;
int a, b, c, d, e, f, h;
long g, i;

void setup() { //setup

  attachInterrupt(digitalPinToInterrupt(2), base, RISING);     //interrupção causada pelo subida do pulso gerado pelo 555

  lcd.begin(16, 2);     // configuração inicial que declara o tipo de LCD
  lcd.createChar(1, relogio);

  Wire.begin();


  pinMode(b1, INPUT_PULLUP); //aumenta
  pinMode(b2, INPUT_PULLUP); //diminui
  pinMode(b3, INPUT_PULLUP); //confirma
  pinMode(led, OUTPUT);


}

int menu = 1;
char pisca = 1;

void loop() {


  DateTime now = RTC.now();

  hora = int(now.hour());
  minuto = int(now.minute());
  dia_rtc = int (now.day());
  mes_rtc = int (now.month());
  ano_rtc = int (now.year());


  char mais = digitalRead(b1);
  char menos = digitalRead(b2);
  char confirma = digitalRead(b3);


  if (confirma == 0)
  {
    menu++;
    delay(200);

  }



  switch (menu)
  {


    case 1:

      a = (12 - mes_rtc) / 10;
      b = ano_rtc - a;
      c = mes_rtc + (12 * a);
      d = b / 100;
      e = d / 4;
      f = 2 - d + e;
      g = (365.25 * b);
      h = (30.6001 * (c + 1));
      i = f + g + h + dia_rtc + 5;
      dia = i % 7;




      clock_();
      contagem();

      lcd.setCursor(5, 1);
      lcd.print(dez_hora);
      lcd.print(uni_hora);
      lcd.setCursor(7, 1);
      if (pisca == 0)
      {
        lcd.print(":");
      }

      else
      {
        lcd.print(" ");
      }
      lcd.setCursor(8, 1);
      lcd.print(dez_minuto);
      lcd.print(uni_minuto);





      switch (dia) {    //dia da semana do relógio


        case 0:
          lcd.setCursor(0, 1);
          lcd.print("SAT  ");
          break;
        case 1:
          lcd.setCursor(0, 1);
          lcd.print("SUN  ");
          break;
        case 2:
          lcd.setCursor(0, 1);
          lcd.print("MON  ");
          break;
        case 3:
          lcd.setCursor(0, 1);
          lcd.print("TUE  ");
          break;
        case 4:
          lcd.setCursor(0, 1);
          lcd.print("WED  ");
          break;
        case 5:
          lcd.setCursor(0, 1);
          lcd.print("THU  ");
          break;
        case 6:
          lcd.setCursor(0, 1);
          lcd.print("FRI  ");
          break;


      }


      break;


    case 2: //timer on hora

      lcd.setCursor(0, 0);
      lcd.print(" TIMER ON      ");
      lcd.setCursor(0, 1);
      lcd.print("   ");//apagar dia da semana

      if (mais == 0)
      {
        hora_on++;
        if (hora_on == 24)
          hora_on = 0;
        delay(200);
      }

      if (menos == 0 && hora_on > 0)
      {
        hora_on--;
        delay(200);
      }

      separa_on();


      if (pisca == 0) {
        lcd.setCursor(5, 1);
        lcd.print(dez_hora_on);
        lcd.print(uni_hora_on);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(" ");
        lcd.print(" ");
      }

      lcd.print(":");
      lcd.print(dez_minuto_on);
      lcd.print(uni_minuto_on);

      break;

    case 3://timer on minutos

      if (mais == 0)
      {
        minuto_on++;
        if (minuto_on == 60)
        {
          minuto_on = 0;
          hora_on++;
        }
        delay(200);
      }

      if (menos == 0 && minuto_on > 0)
      {
        minuto_on--;
        delay(200);
      }

      separa_on();


      lcd.setCursor(5, 1);
      lcd.print(dez_hora_on);
      lcd.print(uni_hora_on);
      lcd.print(":");
      if (pisca == 0) {
        lcd.print(dez_minuto_on);
        lcd.print(uni_minuto_on);
      }
      else
      {
        lcd.setCursor(8, 1);
        lcd.print(" ");
        lcd.print(" ");
      }
      break;


    case 4: //timer off hora

      lcd.setCursor(1, 0);
      lcd.print("TIMER OFF     ");

      if (mais == 0)
      {
        hora_off++;
        if (hora_off == 24)
          hora_off = 0;
        delay(200);
      }

      if (menos == 0 && hora_off > 0)
      {
        hora_off--;
        delay(200);
      }

      separa_off();


      if (pisca == 0) {
        lcd.setCursor(5, 1);
        lcd.print(dez_hora_off);
        lcd.print(uni_hora_off);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(" ");
        lcd.print(" ");
      }

      lcd.print(":");
      lcd.print(dez_minuto_off);
      lcd.print(uni_minuto_off);

      break;

    case 5://timer off minutos

      if (mais == 0)
      {
        minuto_off++;
        if (minuto_off == 60)
        {
          minuto_off = 0;
          hora_off++;
        }
        delay(200);
      }

      if (menos == 0 && minuto_off > 0)
      {
        minuto_off--;
        delay(200);
      }

      separa_off();


      lcd.setCursor(5, 1);
      lcd.print(dez_hora_off);
      lcd.print(uni_hora_off);
      lcd.print(":");
      if (pisca == 0) {
        lcd.print(dez_minuto_off);
        lcd.print(uni_minuto_off);
      }
      else
      {
        lcd.setCursor(8, 1);
        lcd.print(" ");
        lcd.print(" ");
      }
      break;

    case 6: //dias da semana do timer

      lcd.setCursor(1, 0);
      lcd.print("SET TIMER DAY  ");

      if (mais == 0 && dia_timer < 10)
      {
        dia_timer++;
        delay(200);
      }

      if (menos == 0 && dia_timer > 1)
      {
        dia_timer--;
        delay(200);
      }

      switch (dia_timer) {

        case 1:
          lcd.setCursor(0, 1);
          lcd.print("SUN          ");
          break;
        case 2:
          lcd.setCursor(0, 1);
          lcd.print("MON          ");
          break;
        case 3:
          lcd.setCursor(0, 1);
          lcd.print("TUE          ");
          break;
        case 4:
          lcd.setCursor(0, 1);
          lcd.print("WED          ");
          break;
        case 5:
          lcd.setCursor(0, 1);
          lcd.print("THU          ");
          break;
        case 6:
          lcd.setCursor(0, 1);
          lcd.print("FRI          ");
          break;
        case 7:
          lcd.setCursor(0, 1);
          lcd.print("SAT          ");
          break;
        case 8:
          lcd.setCursor(0, 1);
          lcd.print("M T W T F ");
          break;
        case 9:
          lcd.setCursor(0, 1);
          lcd.print("M T W T F S  ");
          break;
        case 10:
          lcd.setCursor(0, 1);
          lcd.print("S M T W T F S");
          break;
      }

      break;

    case 7: //mostra o relógio

      clock_();
      if (hora_on == hora_off && minuto_on == minuto_off) {
        lcd.setCursor(15, 1);
        lcd.print(" ");
      }
      else {
        lcd.setCursor(15, 1);
        lcd.write(1);
      }

      lcd.setCursor(10, 1);
      lcd.print("   ");

      switch (dia) {    //dia da semana do relógio

          if (hora == 0 && minuto == 0) {
            dia = dia++;
          }

        case 1:
          lcd.setCursor(0, 1);
          lcd.print("SUN  ");
          break;
        case 2:
          lcd.setCursor(0, 1);
          lcd.print("MON  ");
          break;
        case 3:
          lcd.setCursor(0, 1);
          lcd.print("TUE  ");
          break;
        case 4:
          lcd.setCursor(0, 1);
          lcd.print("WED  ");
          break;
        case 5:
          lcd.setCursor(0, 1);
          lcd.print("THU  ");
          break;
        case 6:
          lcd.setCursor(0, 1);
          lcd.print("FRI  ");
          break;
        case 7:
          lcd.setCursor(0, 1);
          lcd.print("SAT  ");
          break;


      }



      contagem();

      lcd.setCursor(5, 1);
      lcd.print(dez_hora);
      lcd.print(uni_hora);
      lcd.setCursor(7, 1);
      if (pisca == 0)
      {
        lcd.print(":");
      }

      else
      {
        lcd.print(" ");
      }
      lcd.setCursor(8, 1);
      lcd.print(dez_minuto);
      lcd.print(uni_minuto);



      if (hora == hora_on && minuto == minuto_on && ((dia == dia_timer) || (dia_timer == 8 && dia > 1) && (dia_timer == 8 && dia < 7) || (dia_timer == 9 && dia > 1) && (dia_timer == 9 && dia <= 7) || dia_timer == 10))
      {
        digitalWrite(led, HIGH);

      }
      if (hora == hora_off && minuto == minuto_off && ((dia == dia_timer) || (dia_timer == 8 && dia > 1) && (dia_timer == 8 && dia < 7) || (dia_timer == 9 && dia > 1) && (dia_timer == 9 && dia <= 7) || dia_timer == 10))
      {
        digitalWrite(led, LOW);

      }

      break;


    case 8:

      menu = 2;

      break;
  }
}

void base()
{
  // segundo++;
  pisca = !pisca;
  contagem();

}

void contagem()
{
  if (segundo == 60)
  { minuto++;
    segundo = 0;

    if (minuto == 60)
    {
      hora++;
      minuto = 0;
      if (hora == 24)
      {
        dia++;
        hora = 0;
        if (dia == 8)
        {
          dia = 1;
        }
      }
    }
  }

  dez_hora = hora / 10;
  uni_hora = hora % 10;
  dez_minuto = minuto / 10;
  uni_minuto = minuto % 10;


}

void separa_on()
{


  dez_hora_on = hora_on / 10;
  uni_hora_on = hora_on % 10;
  dez_minuto_on = minuto_on / 10;
  uni_minuto_on = minuto_on % 10;


}
void separa_off()
{


  dez_hora_off = hora_off / 10;
  uni_hora_off = hora_off % 10;
  dez_minuto_off = minuto_off / 10;
  uni_minuto_off = minuto_off % 10;


}

void clock_() {
  lcd.setCursor(0, 0);
  lcd.print("CLOCK");
  lcd.print(" ");
  lcd.print(dia_rtc);
  lcd.print("/");
  lcd.print(mes_rtc);
  lcd.print("/");
  lcd.print(ano_rtc);
}
