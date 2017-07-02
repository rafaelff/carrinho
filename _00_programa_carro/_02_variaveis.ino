double erro = 0;
double erro_PID = 0;
PID myPID(&erro, &erro_PID, 0, PID_P, PID_I, PID_D, REVERSE);

enum tipoEstado {emMovimento, curva90, aguardandoBraco, obstaculoDireita, obstaculoEsquerda};
tipoEstado EstadoAtual = emMovimento;
int vel_esq = 100;
int vel_dir = 100;
int vel_geral = 100;
int tempoAnterior[] = {0, 0, 0, 0};
int tempoDecorrido[] = {0, 0, 0, 0};

int referencia;
int ref_esq;
int ref_dir;
int dir_curva90 = 0; //0 nada; +1 direita; -1 esquerda;
bool debounce = false;
