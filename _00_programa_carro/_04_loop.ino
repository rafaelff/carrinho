void loop() {
  // Verifica a contagem do tempo
  int tempoAtual = millis();
  for(int i=0;i<=3;i++) {
    tempoDecorrido[i] = tempoAtual - tempoAnterior[i];  
  }

  int sensor = analogRead(SWLINHA_B);
  if((EstadoAtual == emMovimento) && (tempoDecorrido[0] >= TEMPO_SENSORES)) {
    if(abs(analogRead(SWDIR) - ref_dir) >= SP_SENSOR) {
    //if(digitalRead(SWDIR)) {
      EstadoAtual = obstaculoDireita;
    } else
    if(abs(analogRead(SWESQ) - ref_esq) >= SP_SENSOR) {
    //if(digitalRead(SWESQ)) {
      EstadoAtual = obstaculoEsquerda;
    } else
    if(abs(referencia - sensor) > SP_SENSOR_REF) {
      tempoAnterior[3] = millis();
      tempoDecorrido[3] = 0;
      EstadoAtual = curva90;
    } else {
      SegueLinha();
    }
    tempoAnterior[0] = millis();
  }

  // Processa a cada 500 milisegundos a atualização da leitura do braço
  if((EstadoAtual == aguardandoBraco) && (tempoDecorrido[1] >= TEMPO_RETORNO_BRACO)) {
    // Verifica se recebeu leitura do braço
    if(digitalRead(ENTRADA_BRACO)) {
      EstadoAtual = emMovimento;
    }
    tempoAnterior[1] = millis();
  }

  // Processa a cada 10 milisegundos a atualização de estado
  if(tempoDecorrido[2] >= TEMPO_ATUALIZA_ESTADO) {
    ProcessaEstado();
    tempoAnterior[2] = millis();
  }
}
