#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA
#include <LiquidCrystal.h>

#define CURRENT_CAL 18.40 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)
const int pinoSensorC0 = A0; 
const int pinoSensorC1 = A1; 
const int pinoSensorC2 = A2; 
const int pinoSensorC3 = A3; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
const int pinoSensorA0 = A4; 
const int pinoSensorA1 = A5; 
const int pinoSensorA2 = A6; 
const int pinoSensorA3 = A7;

EnergyMonitor emon0;
EnergyMonitor emon1;
EnergyMonitor emon2;
EnergyMonitor emon3; //CRIA UMA INSTÂNCIA
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float v1;
float v2;
float v3;
float v4;

void setup(){  
  //Serial.begin(9600); //INICIALIZA A SERIAL
  emon0.current(pinoSensorC0, CURRENT_CAL); 
  emon1.current(pinoSensorC1, CURRENT_CAL); 
  emon2.current(pinoSensorC2, CURRENT_CAL); 
  emon3.current(pinoSensorC3, CURRENT_CAL); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO)
  lcd.begin(20, 4);
}

void loop(){
  float max1 = 0.0;
  float acPeakVoltage1 = 0;
  float max2 = 0.0;
  float acPeakVoltage2 = 0;
  float max3 = 0.0;
  float acPeakVoltage3 = 0;
  float max4 = 0.0;
  float acPeakVoltage4 = 0;
  v1 = analogRead(A4);
  v2 = analogRead(A5);
  v3 = analogRead(A6);
  v4 = analogRead(A7);
  v1 = (v1) * 0.0048828;
  v1 *= 1000;
  v2 = (v2) * 0.0048828;
  v2 *= 1000;
  v3 = (v3) * 0.0048828;
  v3 *= 1000;
  v4 = (v4) * 0.0048828;
  v4 *= 1000;
  //v1 = (v1 - 5.0);
  max1 =  (v1) * 60;
  max2 =  (v2) * 60;
  max3 =  (v3) * 60;
  max4 =  (v4) * 60;
  
  acPeakVoltage1 =  ((max1) * 0.707);
  acPeakVoltage1 = acPeakVoltage1 / 1000;
  acPeakVoltage2 =  ((max2) * 0.707);
  acPeakVoltage2 = acPeakVoltage2 / 1000;
  acPeakVoltage3 =  ((max3) * 0.707);
  acPeakVoltage3 = acPeakVoltage3 / 1000;
  acPeakVoltage4 =  ((max4) * 0.707);
  acPeakVoltage4 = acPeakVoltage4 / 1000;
  
  emon0.calcVI(17, 100);
  emon1.calcVI(17, 100);
  emon2.calcVI(17, 100);
  emon3.calcVI(17, 100); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  double currentDraw0 = emon0.Irms;
  double currentDraw1 = emon1.Irms;
  double currentDraw2 = emon2.Irms;
  double currentDraw3 = emon3.Irms;
  double todasCorrentes[] = {currentDraw0, currentDraw1, currentDraw2, currentDraw3};
  float todasMaxTensao[] = {acPeakVoltage1, acPeakVoltage2, acPeakVoltage3, acPeakVoltage4};
  double avgCurrentDraw = (currentDraw0 + currentDraw1 + currentDraw2 + currentDraw3) / 4; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
  lcd.clear();
  char tensao[5];
  char corrente[5];
  float potencia;
  char potenciaC[5];
  for (int i = 0;  i < 2; i++){
    

    if(todasCorrentes[i] >= 10.0)
      dtostrf(todasCorrentes[i], 3, 1, corrente);
    else
      dtostrf(todasCorrentes[i], 3, 2, corrente);

    if(todasMaxTensao[i] >= 100.0)
      dtostrf(todasMaxTensao[i], -4, 0, tensao);
    else
      dtostrf(todasMaxTensao[i], -4, 1, tensao);

    potencia = todasMaxTensao[i] * todasCorrentes[i];
    if(potencia >= 100.0)
      dtostrf(potencia, -4, 0, potenciaC);
    else
      dtostrf(potencia, -4, 1, potenciaC);

    if(i == 2){
      lcd.clear();
    }
    lcd.setCursor((i * 9), 0);
    lcd.print("Area ");
    lcd.print(i + 1);
    lcd.print(":");
    lcd.setCursor((i * 9), 1);
    lcd.print(corrente);
    lcd.print("A");
    lcd.setCursor((i * 9), 2);
    lcd.print(tensao);
    lcd.print("V");
    lcd.setCursor((i * 9), 3);
    lcd.print(potenciaC);
    lcd.print("W");
  }
  delay(2000);
  lcd.clear();
  for (int i = 2;  i < 4; i++){
    

    if(todasCorrentes[i] >= 10.0)
      dtostrf(todasCorrentes[i], 3, 1, corrente);
    else
      dtostrf(todasCorrentes[i], 3, 2, corrente);

    if(todasMaxTensao[i] >= 100.0)
      dtostrf(todasMaxTensao[i], -4, 0, tensao);
    else
      dtostrf(todasMaxTensao[i], -4, 1, tensao);

    potencia = todasMaxTensao[i] * todasCorrentes[i];
    if(potencia >= 100.0)
      dtostrf(potencia, -4, 0, potenciaC);
    else
      dtostrf(potencia, -4, 1, potenciaC);
    lcd.setCursor((i * 9 - 18), 0);
    lcd.print("Area ");
    lcd.print(i + 1);
    lcd.print(":");
    lcd.setCursor((i * 9 - 18), 1);
    lcd.print(corrente);
    lcd.print("A");
    lcd.setCursor((i * 9 - 18), 2);
    lcd.print(tensao);
    lcd.print("V");
    lcd.setCursor((i * 9 - 18), 3);
    lcd.print(potenciaC);
    lcd.print("W");
  }

  
   //IMPRIME O TEXTO NA SERIAL
}
