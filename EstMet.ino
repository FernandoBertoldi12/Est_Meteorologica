/*                                                
                                                         UNIVERSIDADE FEDERAL DO PAMPA

ALUNOS: Eduarda Menezes da Silveira & Fernando Antônio Alves Bertoldi
PROFESSOR: Julio Saracol Domingues Junior

                                                             Estação Metereológica
*/

#define BLYNK_TEMPLATE_ID "TMPLSuR-uJgY"
#define BLYNK_DEVICE_NAME "Estação Meteorológica"
#define BLYNK_AUTH_TOKEN "D7Xz1DwZ2Jf9QomG-aJv7lD9VV521jiS"
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>   //iNCLUSÃO DA BIBLIOTECA DO DISPLAY LCD
#include "DHT.h"            //INCLUSÃO DA BIBLIOTECA DO SENSOR
#define DHTTYPE DHT22      //DHT22 sensor is used
#define DHTPIN 40          //PINO DIGITAL UTILIZADO PELO DHT22
#define RATIO 11.93
DHT dht(DHTPIN, DHTTYPE);//Inicia a biblioteca DHT
const int pinoSensor = A1; //PINO UTILIZADO PELO SENSOR
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO
int analogSoloSeco = 500; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 450; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)
int pinoAnalogico = A2; //PINO ANALÓGICO (MEDE A INTENSIDADE DO CAMPO MAGNÉTICO)
int pinoDigital = 41; //PINO DIGITAL (DETECTA A PRESENÇA DO CAMPO MAGNÉTICO)
int counter   = 0; //acumulador
char auth[] = BLYNK_AUTH_TOKEN;
// PINOS UTILIZADOS PELOS MÓDULOS DO LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(38, 39, 34, 35, 36, 37);
uint32_t timer0 = 0;
uint32_t timer1 = 0;
uint32_t timer2 = 0;
uint32_t timer3 = 0;
bool go = false;
float anemometer = 0; //velocidade

void meter(){
    //interrupcao a cada 2segs, portanto divide o tempo
    //para ter cm/s
    anemometer = (counter * RATIO)/2;  
    counter    = 0; 
    go         = true;
}

BlynkTimer timer;

BLYNK_WRITE(V0){

   int value = param.asInt();
  digitalWrite(dht.readTemperature(),value);

}

BLYNK_WRITE(V1){

   int value = param.asInt();
  digitalWrite(dht.readHumidity(),value);

}

BLYNK_WRITE(V2){

   int value = param.asInt();
  digitalWrite(valorLido,value);

}

BLYNK_WRITE(V3){

   int value = param.asInt();
  digitalWrite(anemometer,value);

}
  
void setup(){  

 
 Timer1.initialize(1000000);
 Timer1.attachInterrupt(meter);
 pinMode(pinoAnalogico, INPUT); //DEFINE O PINO COMO ENTRADA
 pinMode(pinoDigital, INPUT); //DEFINE O PINO COMO ENTRADA
 lcd.begin(16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY. EM SUMA: UMA MATRIZ DE 16 COLUNAS E 2 LINHAS
 lcd.setCursor(0,0); //SETA A POSIÇÃO EM QUE O CURSOR INCIALIZA(LINHA 1) 
 lcd.print("Estacao Met.."); //ESCREVE A FRASE "APERTOU A TECLA" NA PRIMEIRA LINHA DO DISPLAY LCD  
 dht.begin();
 //Blynk.begin(auth);
}  
  
void loop(){  

 //Blynk.run();

 lcd.setCursor(0,0); //SETA A POSIÇÃO EM QUE O CURSOR RECEBE O TEXTO A SER MOSTRADO(LINHA 2)

 if ((analogRead(0)) < 80) { //SE A LEITURA DO PINO FOR MENOR QUE 80, FAZ 
    
    //lcd.clear(); // limpa a tela do display
    
     if(digitalRead(pinoDigital) == HIGH){  
        counter += 1;
        delay(80);
    } 
     
    if(go){

      Serial.print(" Centimetros por segundo: ");  
      Serial.print(anemometer);
      Serial.println(" cm/s");
      lcd.setCursor(0,0); // Define o cursor na posição de início
      lcd.print("VelVent:");
      lcd.print(anemometer);
      lcd.print("cm/s");

      go = false;
    }

 }  
 else if ((analogRead(0)) < 200) { //SE A LEITURA DO PINO FOR MENOR QUE 200, FAZ
           
    if(millis() - timer1>= 2000)  {  // Executa 1 vez a cada 2 segundos
    lcd.clear(); // limpa a tela do display
    // Exibe no monitor o valor da temperatura
    Serial.print("Temperatura = ");
    Serial.print(dht.readTemperature()); 
    Serial.print(" Celsius  ");
   
    // Exibe no display LCD o valor da temperatura
    lcd.setCursor(0,0); // Define o cursor na posição de início
    lcd.print("Temp(Ar):");
    lcd.print(dht.readTemperature());
    lcd.write(B11011111); // Imprime o símbolo de grau
    lcd.print("C");

    timer1 = millis(); // Atualiza a referência
  }

 }  
 else if ((analogRead(0)) < 400){ //SE A LEITURA DO PINO FOR MENOR QUE 400, FAZ  
    
    
    if(millis() - timer0>= 2000)  {  // Executa 1 vez a cada 2 segundos
    lcd.clear(); // limpa a tela do display    
    // Exibe no monitor o valor de umidade
    Serial.print("Umidade = ");
    Serial.print(dht.readHumidity());
    Serial.println(" %  ");
    
    // Exibe no display LCD o valor da humidade
    lcd.setCursor(0,0); // Define o cursor na posição de início
    lcd.print("Umi(Ar):");
    lcd.print(dht.readHumidity());
    lcd.print("%");

    timer0 = millis(); // Atualiza a referência
  }

 }  
 else if ((analogRead(0)) < 600){ //SE A LEITURA DO PINO FOR MENOR QUE 600, FAZ  
 
  if(millis() - timer2>= 2000)  {  // Executa 1 vez a cada 2 segundos
   
    lcd.clear(); // limpa a tela do display
    valorLido = constrain(analogRead(pinoSensor),analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
    valorLido = map(valorLido,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
    Serial.print("Umidade do solo: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(valorLido); //IMPRIME NO MONITOR SERIAL O PERCENTUAL DE UMIDADE DO SOLO
    Serial.println("%"); //IMPRIME O CARACTERE NO MONITOR SERIAL  
    lcd.setCursor(0,0); // Define o cursor na posição de início
    lcd.print("Umi(solo):");
    lcd.print(valorLido);
    lcd.print("%");

    timer2 = millis(); // Atualiza a referência
 
  }
 
 }  

 else if ((analogRead(0)) < 800){ //SE A LEITURA DO PINO FOR MENOR QUE 800, FAZ 

  lcd.print ("     SELECT    "); //ESCREVE NO LCD O TEXTO 

 }  
 
}