float controle_vel_linear(float vel_desejada, float vel_medida) {

  // Parâmetros do controlador
  float kp = 0.01;    // Ganho Proporcional (ajuste conforme necessário)
  float ki = 0.0001;  // Ganho Integral (ajuste conforme necessário)

  // Variáveis de estado
  static float prevError = 0.0;
  static float integral = 0.0;
  static unsigned long prevTime = 0;
  static float prevControle = 0.0;

  // Tempo de amostragem em milissegundos
  unsigned long sampleTime = 500;

  // Obtém o tempo atual
  unsigned long tempoAtual = millis();

  // Verifica se é hora de realizar um novo cálculo de controle
  if (tempoAtual - prevTime >= sampleTime) {
    prevTime = tempoAtual;

    // Calcula o erro atual
    float error = vel_desejada - vel_medida;

    // Calcula a parte proporcional do controle
    float proporcional = kp * error;

    // Calcula a parte integral do controle
    integral += ki * error;

    // Soma as partes proporcional e integral para obter o controle total
    float Controle = proporcional + integral;

    // Limita a saída do controle para evitar valores muito altos ou baixos
    if (Controle > 0.5) {
      Controle = 0.5;
    } else if (Controle < -0.6) {
      Controle = -0.6;
    }

    // Armazena o controle anterior para uso na próxima iteração
    prevControle = Controle;
    
    // Armazena o erro anterior para cálculo da parte integral
    prevError = error;

    // Saída de depuração (pode ser comentada quando não for necessário)
    Serial.print("vel_desejada: " + String(vel_desejada));
    Serial.print("   vel_medida: " + String(vel_medida));
    Serial.println("   Controle: " + String(Controle));

    return Controle;
  }

  // Se não é hora de calcular o controle, retorna o valor anterior
  return prevControle;
}
