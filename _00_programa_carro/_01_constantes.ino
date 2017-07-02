#include <PID_v1.h>

#define SWDIR A4 //pino do sensor de obstáculo à direita
#define SWESQ A0 //pino do sensor de obstáculo à esquerda
#define SP_SENSOR 100 //set point dos sensores de obstáculo

#define SAIDA_DIR 53 //pino de saída do sinal de obstáculo à direita detectado
#define SAIDA_ESQ 51 //pino de saída do sinal de obstáculo à esquerda detectado
#define ENTRADA_BRACO 49 //pino de entrada do sinal do braço

#define SWLINHA_A A1 //pino de entrada do sensor de linha esquerdo
#define SWLINHA_B A2 //pino de entrada do sensor de linha central (de referência)
#define SWLINHA_C A3 //pino de entrada do sensor de linha direito
#define SP_SENSOR_REF 800 //valor da diferença entre leitura atual e anterior do sensor de referência que caracteriza mudança de estado
#define SP_SENSOR_90 50 //valor da diferença entre leitura dos sensores de linha que caracteriza curva de 90º

#define DIR_MOTOR_DIR_B 23 //pino de saída de controle de direção do motor direito
#define DIR_MOTOR_DIR_A 25 //pino de saída de controle de direção do motor direito
#define DIR_MOTOR_ESQ_B 27 //pino de saída de controle de direção do motor esquerdo
#define DIR_MOTOR_ESQ_A 29 //pino de saída de controle de direção do motor esquerdo
#define PWM_MOTOR_DIR 2 //pino de saída de controle de velocidade do motor direito
#define PWM_MOTOR_ESQ 3 //pino de saída de controle de velocidade do motor esquerdo

#define MAXPWM 255 //valor máximo permitido para o PWM (padrão: 255)
#define MINPWM 150 //valor mínimo para o PWM vencer a inércia
#define AVANCO_CURVA_90 6000 //tempo de avanço após detectar uma curva de 90º para alinhar com o centro do carro (milisegundos)
#define TEMPO_SENSORES 50 //tempo de atualização dos sensores (milisegundos)
#define TEMPO_RETORNO_BRACO 500 //tempo de atualização do sinal de retorno do braço (milisegundos)
#define TEMPO_ATUALIZA_ESTADO 10 //tempo de atualização do estado (milisegundos)

#define PID_P 1 //constante proporcional do controle PID
#define PID_I 0 //constante integral do controle PID
#define PID_D 0.01 //constante derivativa do controle PID
