#include <Ticker.h>

int spValue = 1080, pvValue = 0;
int sensorValue = 0;

const int pin_mv = 4; //pino led
const int pin_pv = 15; // Pino de entrada para a variável medida (ldr)


const int lim_int_sup = 29000;
const int lim_int_inf = -29000;
const int lim_pwm_sup = 254;

volatile int teste = 0, teste2 = 0, display_7seg;
float kp = 0.01;  // Constante ganho proporcional
float Ti = 0.8;   // Tempo Integral
float Td = 0.1;   // Tempo Derivativo
float erro = 0;   // Erro
float erro_ant = 0; // Erro anterior para ação derivativa
float P = 0;    // Ação Proporcional
float S = 0;    // Integrador
float I = 0;    // Ação Integral
float D = 0;    // Ação Derivativa
float acao;     // Ação na variável manipulada

Ticker timer;

void amostragem()
{
  teste++;
  teste2++;
  if (teste > 30)
  {
    teste = 0;
    pvValue = analogRead(pin_pv); // Leitura do valor de variável medida
    erro = pvValue - spValue;
    P = erro * kp;
    S = S + erro;    // Fórmula do integrador
    if (S > lim_int_sup) // Limitador Integral superior
    {
      S = lim_int_sup;
    }
    if (S < lim_int_inf) // Limitador Integral inferior
    {
      S = lim_int_inf;
    }
    I = kp * S / Ti; // Fórmula da ação integral
    D = (erro_ant - erro) * Td;
    erro_ant = erro;
    
    acao = P + I + D;   // Cálculo da ação final do controlador PID
    if (acao > lim_pwm_sup) // Limitador saída superior pwm
    {
      acao = lim_pwm_sup;
    }
    if (acao < 0) // Limitador saída inferior pwm
    {
      acao = 0;
    }
    analogWrite(pin_mv, acao); // Linha principal para ação na variável manipulada
  }
  if (teste2 > 1000) // Atualiza o gráfico do plotter serial a cada 1 segundo
  {
    teste2 = 0;
    Serial.println(); // As linhas a seguir servem para mostrar as variáveis no plotter serial do Arduino
    Serial.print("sp=");
    Serial.print(spValue); // Legenda setpoint
    Serial.print(" pv=");
    Serial.print(pvValue); // Legenda variável medida
    Serial.print(" mv=");
    Serial.print(acao); // Legenda variável manipulada
    Serial.print(" erro=");
    Serial.print(erro); // Legenda desvio ou erro
  }
}

void setup()
{
  Serial.begin(9600); // Define velocidade serial de 9600bps
  pinMode(pin_mv, OUTPUT);
  pinMode(10, OUTPUT);
  timer.attach_ms(1, amostragem); // Configura uma interrupção a cada 1 ms
  delay(100);
}

void loop()
{
 
}