void setup() {
  pinMode(SWDIR, INPUT);
  pinMode(SWESQ, INPUT);

  pinMode(SAIDA_DIR, OUTPUT);
  pinMode(SAIDA_ESQ, OUTPUT);
  pinMode(ENTRADA_BRACO, INPUT);
  
  pinMode(DIR_MOTOR_DIR_A, OUTPUT);
  pinMode(DIR_MOTOR_DIR_B, OUTPUT);
  pinMode(DIR_MOTOR_ESQ_A, OUTPUT);
  pinMode(DIR_MOTOR_ESQ_B, OUTPUT);
  pinMode(PWM_MOTOR_DIR, OUTPUT);
  pinMode(PWM_MOTOR_ESQ, OUTPUT);

  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(TEMPO_SENSORES);
  myPID.SetOutputLimits(-100,100);

  //pequeno delay para garantir que o carro está na pista antes de definir o valor do sensor de referência
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(4000);
  digitalWrite(LED_BUILTIN, LOW);
  
  referencia = analogRead(SWLINHA_B);
  ref_esq = analogRead(SWESQ);
  ref_dir = analogRead(SWDIR);

}

void ProcessaEstado() {
  switch(EstadoAtual) {
    case emMovimento:
      digitalWrite(SAIDA_DIR, LOW);
      digitalWrite(SAIDA_ESQ, LOW);
      break;
    case aguardandoBraco:
      //reservado para acender um LED ou executar outra ação
      break;
    case obstaculoEsquerda:
      digitalWrite(SAIDA_ESQ, HIGH);
      Parar();
      EstadoAtual = aguardandoBraco;
      break;
    case obstaculoDireita:
      digitalWrite(SAIDA_DIR, HIGH);
      Parar();
      EstadoAtual = aguardandoBraco;
      break;  
    case curva90:
      Curva90();
      break;
  }
}

void Parar() {
  // Reservado para desenvolver uma parada suave
  vel_esq = 100;
  vel_dir = 100;
  MudaVel();
  digitalWrite(DIR_MOTOR_ESQ_A, LOW);
  digitalWrite(DIR_MOTOR_ESQ_B, LOW);
  digitalWrite(DIR_MOTOR_DIR_A, LOW);
  digitalWrite(DIR_MOTOR_DIR_B, LOW);
}

void MudaVel() {
  if(vel_esq < 0) {
    digitalWrite(DIR_MOTOR_ESQ_A, LOW);
    digitalWrite(DIR_MOTOR_ESQ_B, HIGH);
  } else {
    digitalWrite(DIR_MOTOR_ESQ_A, HIGH);
    digitalWrite(DIR_MOTOR_ESQ_B, LOW);
  }
  if(vel_dir < 0) {
    digitalWrite(DIR_MOTOR_DIR_A, LOW);
    digitalWrite(DIR_MOTOR_DIR_B, HIGH);
  } else {
    digitalWrite(DIR_MOTOR_DIR_A, HIGH);
    digitalWrite(DIR_MOTOR_DIR_B, LOW);
  }
  int pwm_esq = (abs(vel_esq)/100.0 * (MAXPWM - MINPWM)) + MINPWM;
  int pwm_dir = (abs(vel_dir)/100.0 * (MAXPWM - MINPWM)) + MINPWM;
  if(pwm_esq <= MINPWM) {pwm_esq = 0;}
  if(pwm_dir <= MINPWM) {pwm_dir = 0;}
  analogWrite(PWM_MOTOR_ESQ, pwm_esq);
  analogWrite(PWM_MOTOR_DIR, pwm_dir);
}

void CalculaErro() { //erro positivo: curva à direita; erro negativo: curva à esquerda; 0~100
  int corretor = 1;
  if(referencia < SP_SENSOR_REF) corretor = -1; //caso a linha seja branca, inverte o sinal do erro
  int esq = analogRead(SWLINHA_A);
  int dir = analogRead(SWLINHA_C);
  int diff = (dir - esq) * corretor / 10;
  erro = diff;
}

void SegueLinha() {
  CalculaErro();
  myPID.Compute();
  if(vel_esq < 100 || vel_dir >= 100) {vel_esq += (int)erro_PID;}
  if(vel_dir < 100 || vel_esq >= 100) {vel_dir -= (int)erro_PID;}
  if(vel_esq > 100) {vel_esq = 100;}
  if(vel_dir > 100) {vel_dir = 100;}
  if(vel_esq < 0) {vel_esq = 0;}
  if(vel_dir < 0) {vel_dir = 0;}
  MudaVel();
}

void Curva90() {
  if(tempoDecorrido[0] >= TEMPO_SENSORES) {
    int sensor_B = analogRead(SWLINHA_B);
    if(abs(referencia - sensor_B) < SP_SENSOR_REF) { //caso a leitura seja igual à referência, retornou à linha -> seguir movimento normal
      if(debounce) {//tentativa de eliminar leituras falsas devido a imperfeições da pista
        debounce = false;
        dir_curva90 = 0;
        vel_esq = 100;
        vel_dir = 100;
        MudaVel();
        EstadoAtual = emMovimento;
        return;
      } else {
        debounce = true;
      }
    } else {
      debounce = false;
    }
    CalculaErro();
    if((erro > SP_SENSOR_90) && (dir_curva90 == 0)) {
      dir_curva90 = 1; //curva à direita
    }
    if(erro < -SP_SENSOR_90 && (dir_curva90 == 0)) {
      dir_curva90 = -1; //curva à esquerda
    }
    if(tempoDecorrido[3] >= AVANCO_CURVA_90) { //seguir em frente até passar o tempo necessário para se alinhar o centro do carro com a curva
      if(dir_curva90 != 0) { //caso tenha detectado curva, virar na direção correta
        vel_esq = 100 * dir_curva90;
        vel_dir = -100 * dir_curva90;
        MudaVel();
      } else { //caso não tenha detectado curva, a cor da linha mudou -> atualizar referência e seguir movimento normal
        referencia = analogRead(SWLINHA_B);
        EstadoAtual = emMovimento;
      }
    }
    tempoAnterior[0] = millis();
  }
}
