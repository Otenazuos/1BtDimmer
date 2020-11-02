#include <mbed.h>

InterruptIn button(p5);
DigitalOut RedLed(p6);
DigitalOut BlueLed(p7);
PwmOut Intensidade(p8);

Timeout timeout;
Ticker tick;

enum State{DOWN, UP, FALL, MIN, RISE, MAX};

State EstadoAtual;
float Frequencia;
bool EstadoBotao;

// Protótipos das funções:
void inicio();
void BtPress();
void BtRelease();
void MudaEstado();
void PiscaLed();



int main() {
  inicio();
  button.rise(&BtPress);
  button.fall(&BtRelease);
}

void inicio(){
  RedLed = 0;
  BlueLed = 1;
  Intensidade = 1.0;
  EstadoAtual = UP;
}

void BtPress(){
  EstadoBotao = true;
  tick.attach(&MudaEstado, 1.0);
}

void BtRelease(){
  EstadoBotao = false;
  tick.detach();
  if(EstadoAtual == MAX || EstadoAtual == UP || EstadoAtual = FALL){
    if(Intensidade > 0){
      BlueLed = 0;
      RedLed = 1.0;
      EstadoAtual = DOWN;
    }
  }
  else if (EstadoAtual == MIN || EstadoAtual == DOWN || EstadoAtual == RISE){
    if(Intensidade < 1.0){
      BlueLed = 1;
      RedLed = 0;
      EstadoAtual = UP;
    }
  }
}

void MudaEstado(){
  timeout.attach(&PiscaLed, 0.1);
  if(EstadoBotao == true){
    if(EstadoAtual == UP) EstadoAtual = RISE;
    else if(EstadoAtual == DOWN) EstadoAtual = FALL;
    else if(EstadoAtual = FALL){
      if(Intensidade == 0) EstadoAtual = MIN;
      else Intensidade = Intensidade - 0.05;
    }
    else if(EstadoAtual == RISE){
      if(Intensidade == 1.0) EstadoAtual = MAX;
      else Intensidade = Intensidade - 0.05;
    }
    else{
      if(Intensidade >= 1.0) EstadoAtual = MAX;
      else if (Intensidade <= 0) EstadoAtual = MIN;
    }
  }
}

void PiscaLed(){
  if(EstadoAtual == FALL){
    Frequencia = 1;
    RedLed = !RedLed;
  }
  if(EstadoAtual == RISE){
    Frequencia = 1;
    BlueLed = !BlueLed;
  }
  if(EstadoAtual == MIN){
    Frequencia = 0.1;
    RedLed = !RedLed;
  }
  if(EstadoAtual == MAX){
    Frequencia = 0.1;
    BlueLed = !BlueLed;
  }
  timeout.attach(&PiscaLed, Frequencia);
}