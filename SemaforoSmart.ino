// Projeto - A3 
// Microcontroladores e Microprocessadores
// Status: Em desenvolvimento...
// Autor: Wesley Silveira dos Santos 

#include <NewPing.h>

#define TRIGGER_PIN  6    // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     5    // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  //Configuração do sensor ultrassonico 

int lamp[] = {13,12,11,10,9,8};  //Configuração das saídas
int delay_a = 1000; //Delay para o semaforo 1
int delay_b = 1000; //Delay para o semaforo 2
int delay_y = 1000; //Delay para Amarelo 
int delay_r = 1000; //Delay para Vermelhos

int sense = 0;      //Variável para ser usado como referência na leitura das entradas

float freq_s1 = 0;  //Leitura da frequencia de carros quando o Semaforo 1 está Aberto 
float freq_s2 = 0;  //Leitura da frequencia de carros quando o Semaforo 2 está Aberto 

//Variáveis de Datas/Horas:

int dados[24][7]; // declarando uma matriz [linhas][colunas]

int dia;  // Variável para armazenar o Dia Atual
int hora; // Variável para armazenar o Hora Atual

void ConfigPlaca()  //
{
  for(int i=0; i < 6; i++)
  {
    pinMode(lamp[i], OUTPUT);
    digitalWrite(lamp[i], 0);
  }
  Serial.begin(9600);   //Para leitura das entradasm sensores ultrasonicos
}

void SwitchLight()
{
  for(int i=0; i < 6; i++)
  {
    switch(i) //Sequencia das Lampadas
    {
      case 0: //Vermelho 1 e Verde 2
        digitalWrite(lamp[0], 1);   //Acende luz do semaforo
        digitalWrite(lamp[5], 1);   //Acende luz do semaforo
        freq_s2 += ReadInput(delay_a);
        break;
        
      case 1: //Vermelho 1 e Amarelo 2
        digitalWrite(lamp[0], 1);
        digitalWrite(lamp[4], 1);
        freq_s2 += ReadInput(delay_y);
        break;
      
      case 2: //Vermelhos 
        digitalWrite(lamp[0], 1);
        digitalWrite(lamp[3], 1);
        delay(delay_r);
        break;
      
      case 3: //Vermelho 2 e Verde 1
        digitalWrite(lamp[3], 1);
        digitalWrite(lamp[2], 1);
        freq_s1 += ReadInput(delay_b);
        break;
      
      case 4: //Vermelho 2 e Amarelo 1
        digitalWrite(lamp[1], 1);
        digitalWrite(lamp[3], 1);
        freq_s1 += ReadInput(delay_b);
        delay(delay_y);
        break;
      
      case 5: //Vermelhos
        digitalWrite(lamp[0], 1);
        digitalWrite(lamp[3], 1);
        delay(delay_r);
        break;
    }
    
    for(int i=0; i < 6; i++)  //Reiniciar Semaforo/Apagar Luzes
    {
      digitalWrite(lamp[i], 0);
    }
  }
}

void ReadData()   //Lê a informação de Dia e Hora Atual
{
  
}

float ReadInput(int timeWait) //Entrada dos Sensores
{
  int cont = 0;                   //Variável para contagem do número de carros 
  
  for(int i=0; i < timeWait; i++) //Leitura dos carros que estão passando na via em verde.
  {
    bool a = sonar.ping_cm()<=20?true:false;
    bool b = sonar.ping_cm()>=100?true:false;
    
    if((a&&b)&&(sense == 1))      //Condição para retornar valor que o sensor está OK
    {
      cont++;
      sense = 0;
      break;
    } 
    else if((a&&b) == false)      //Condicional para leitura do sensor da roda/disco
    {
      sense = 1;
    }
    delay(1);
  }
  return timeWait/cont;
}

void setup()
{
  ConfigPlaca();
}

void loop()
{
  SwitchLight();
}
