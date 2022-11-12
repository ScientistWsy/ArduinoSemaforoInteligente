// Projeto - A3 
// Microcontroladores e Microprocessadores
// 

//#include <NewPing.h>

int lamp[] = {13,12,11,10,9,8};  //Configuração das saídas
int delay_a = 1000; //Delay para o semaforo 1
int delay_b = 1000; //Delay para o semaforo 2
int delay_y = 1000; //Delay para Amarelo 
int delay_r = 1000; //Delay para Vermelhos

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
  Serial.begin(9600);
}

void SwitchLight()
{
  for(int i=0; i < 6; i++)
  {
    switch(i) //Sequencia das Lampadas
    {
      case 0: //Vermelho 1 e Verde 2
        digitalWrite(lamp[0], 1);
        digitalWrite(lamp[5], 1);
        delay(delay_a);
        break;
        
      case 1: //Vermelho 1 e Amarelo 2
        digitalWrite(lamp[0], 1);
        digitalWrite(lamp[4], 1);
        delay(delay_y);
        break;
      
      case 2: //Vermelhos 
        digitalWrite(lamp[0], 1);
        digitalWrite(lamp[3], 1);
        delay(delay_r);
        break;
      
      case 3: //Vermelho 2 e Verde 1
        digitalWrite(lamp[3], 1);
        digitalWrite(lamp[2], 1);
        delay(delay_b);
        break;
      
      case 4: //Vermelho 2 e Amarelo 1
        digitalWrite(lamp[1], 1);
        digitalWrite(lamp[3], 1);
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

void ReadData()   //Lê a  informação de Dia e Hora Atual
{
  
}

void inputSense() //Entrada dos Sensores
{
   
}

void Learning()   //Definição da prioridade
{
  for(int i=1; i <= 24; i++){ //Faz a varredura do dia e hora atual para pegar a variável de frequência
    for(int j=1; j <= 7; j++){
      if(hora == data[i][0]){
        if(dia == data[0][j]{
          
      }
    }
  }
}

void Learning()   //Definição da prioridade
{
  for(int i=1; i <= 24; i++){ //Faz a varredura do dia e hora atual para pegar a variável de frequência
    for(int j=1; j <= 7; j++){
      if(hora == data[i][0]){
        if(dia == data[0][j]{
          
      }
    }
  }
}           
           
void setup()
{
  ConfigPlaca();
}

void loop()
{
  SwitchLight();
}
