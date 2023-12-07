#ifndef MOTRO_CONTROL_QUEUL_H
#define MOTRO_CONTROL_QUEUL_H
#include "main.h"
#include "usart.h"

#define Motro_UART_Buffer 30

enum{
    Motro_Control_Nomal_Sign=1,
    Motro_Position_Check_Sign,
    Motro_Encoder_Check_Sign,
};

typedef struct{
    UART_HandleTypeDef *huart;
    uint8_t data[10];
    uint8_t Size;
    // uint8_t receive[Motro_UART_Buffer];
    uint8_t Verify[3];
}UART_;

struct Link
{
	UART_ UART;
	struct Link* next;
};
 
struct Queue
{
	struct Link* front;
	struct Link* rear;
	int size;
};

void QueueInit(struct Queue* queue);
int QueueEmpty(struct Queue* queue);
void QueuePush(struct Queue* queue, const UART_ UART);
int QueuePop(struct Queue* queue, UART_* UART);
void QueueDestroy(struct Queue* queue);

#endif
