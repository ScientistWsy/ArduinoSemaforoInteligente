// Projeto - A3 
// Microcontroladores e Microprocessadores

// Desenvolvido por: Wesley Silveira 
// Last version: 07/12/2022

// Bibliotéca para o RTC:

#include <RTClib.h> //Incuido Bibliotéca do RTC.

RTC_DS1307 RTC;

//Definindo os pinos dos sensores Ultrassonicos

#define PIN_TRIG_A 3
#define PIN_ECHO_A 2

#define PIN_TRIG_B 6
#define PIN_ECHO_B 5

int lamp[] = {13,12,11,10,9,8};  //Configuração das saídas

int delay_a = 1000; //Delay para o semaforo 1
int delay_b = 1000; //Delay para o semaforo 2
int delay_y = 1000; //Delay para Amarelo 
int delay_r = 1000; //Delay para Vermelhos

int time_s1[] = {6000,5500,5000,4500,4000,3500,3000,2500,2000,1500,1000};
int time_s2[] = {1000,1500,2000,2500,3000,3500,4000,4500,5000,5500,6000};

int sense = 0;      //Variável para ser usado como referência na leitura das entradas

int freq_s1 = 0;  //Leitura da frequencia de carros quando o Semaforo 1 está Aberto 
int freq_s2 = 0;  //Leitura da frequencia de carros quando o Semaforo 2 está Aberto 

//Variáveis de Datas/Horas:

int dados[24][7]; // declarando uma matriz [linhas][colunas]

int dia;  // Variável para armazenar o Dia Atual
int hora; // Variável para armazenar o Hora Atual

int referencia = 5;   //Variável onde é armazenado a lógica do horário atual

void ConfigPlaca()  //
{
  for(int i=0; i < 6; i++)
  {
    pinMode(lamp[i], OUTPUT);
    digitalWrite(lamp[i], 0);
  }
  
  Serial.begin(9600);   

  //Pinos para leitura/comunicação com os sensores ultrassônicos
  pinMode(PIN_TRIG_A, OUTPUT);
  pinMode(PIN_ECHO_A, INPUT);
  pinMode(PIN_TRIG_B, OUTPUT);
  pinMode(PIN_ECHO_B, INPUT);

  RTC.begin();
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
        freq_s2 += ReadInput2(delay_a);
        break;
        
      case 1: //Vermelho 1 e Amarelo 2
        digitalWrite(lamp[0], 1);   //Acende luz do semaforo
        digitalWrite(lamp[4], 1);   //Acende luz do semaforo
        delay(delay_y);
        break;
      
      case 2: //Vermelhos 
        digitalWrite(lamp[0], 1);   //Acende luz do semaforo
        digitalWrite(lamp[3], 1);   //Acende luz do semaforo
        delay(delay_r);
        break;
      
      case 3: //Vermelho 2 e Verde 1
        digitalWrite(lamp[3], 1);   //Acende luz do semaforo
        digitalWrite(lamp[2], 1);   //Acende luz do semaforo
        freq_s1 += ReadInput(delay_b);
        break;
      
      case 4: //Vermelho 2 e Amarelo 1
        digitalWrite(lamp[1], 1);   //Acende luz do semaforo
        digitalWrite(lamp[3], 1);   //Acende luz do semaforo
        delay(delay_y);
        break;
      
      case 5: //Vermelhos
        digitalWrite(lamp[0], 1);   //Acende luz do semaforo
        digitalWrite(lamp[3], 1);   //Acende luz do semaforo
        delay(delay_r);
        break;
    }
    
    for(int i=0; i < 6; i++)  //Reiniciar Semaforo/Apagar Luzes
    {
      digitalWrite(lamp[i], 0);
    }
  }
}

void Atualiza()
{
  delay_a = time_s1[referencia];
  delay_b = time_s2[referencia];

  freq_s1 = 0;
  freq_s2 = 0;
}

void Controle()   //Função que define a lógica do semaforo
{
  //Verifica qual é o dia e a hora atual e busca a última informação/dado
  DateTime now = RTC.now();

  dia = now.dayOfTheWeek();
  hora = now.hour();

  referencia = dados[hora][dia];

  //Compara as informações coletadas no semaforo 
  int compare = freq_s1 - freq_s2;

  if(compare < 0)
  {
    referencia += 1;

    referencia = (referencia>10)?10:referencia;
  }
  else if(compare > 0)
  {
    referencia -= 1;

    referencia = (referencia<0)?0:referencia;
  }
  
  dados[hora][dia] = referencia;
}

int ReadInput(int timeWait) //Entrada dos Sensor 1
{
  int cont = 0;                   //Variável para contagem do número de carros 

  unsigned int i = timeWait;

  while(i > 0)
  //for(int i=0; i < timeWait; i++) //Leitura dos carros que estão passando na via em verde.
  {
    //Inicia uma nova medição do sensor ultrassônico
    digitalWrite(PIN_TRIG_A, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG_A, LOW);

    //Lê o resultado 
    int duration = pulseIn(PIN_ECHO_A, HIGH);

    //Transforma a medição de duração para centimetros
    int cm = (duration/58);

    bool a = (cm >= 20)?true:false;
    bool b = (cm <= 100)?true:false;

    if((a&&b)&&(sense == 1))      //Condição para retornar valor que o sensor está OK
    {
      cont += 1;
      sense = 0;
    } 
    else if((a&&b) == false)      //Condicional para leitura do sensor da roda/disco
    {
      sense = 1;
    }
    i--;    
    delay(1);
  }

  if(cont == 0) //Condicional para evitar divisão por Zero
  {
    return 15000;
  }
  else
  {
    return timeWait/cont;
  }
}

int ReadInput2(int timeWait) //Entrada dos Sensor 2
{
  int cont = 0;                   //Variável para contagem do número de carros 

  unsigned int i = timeWait;

  while( i > 0)
  //for(int i=0; i < timeWait; i++) //Leitura dos carros que estão passando na via em verde.
  {
    //Inicia uma nova medição do sensor ultrassônico
    digitalWrite(PIN_TRIG_B, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG_B, LOW);

    //Lê o resultado 
    int duration = pulseIn(PIN_ECHO_B, HIGH);

    //Transforma a medição de duração para centimetros
    int cm = (duration/58);
    
    //Verifica a distância de controle "Se tem Veículo"
    bool a = (cm >= 20)?true:false;
    bool b = (cm <= 100)?true:false;

    if((a&&b)&&(sense == 1))      //Condição para retornar valor que o sensor está OK
    {
      cont++;
      sense = 0;
    } 
    else if((a&&b) == false)      //Condicional para leitura do sensor da roda/disco
    {
      sense = 1;
    }
    i--;
    delay(1);
  }

  if(cont == 0) //Condicional para evitar divisão por Zero
  {
    return 15000;
  }
  else
  {
    return timeWait/cont;
  }
}

void setup()
{
  ConfigPlaca();
}

void loop()
{
  SwitchLight();
  Controle();
  Atualiza();
  
  Serial.print("A: ");
  Serial.println(delay_a);    //Apenas o monitoramento das variáveis durante os testes

  Serial.print("B: ");
  Serial.println(delay_b);    //Apenas o monitoramento das variáveis durante os testes

  Serial.print("");
}
