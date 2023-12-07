#include "Motro_Control_Queul.h"
#include <stdlib.h>
 
void QueueInit(struct Queue* queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}
 
int QueueEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

void QueuePush(struct Queue* queue, const UART_ UART)
{
    struct Link* node;
    node = (struct Link*)malloc(sizeof(struct Link));
    
    node->UART = UART;
    node->next = NULL;
    
    if(QueueEmpty(queue))
    {
        queue->front = node;
        queue->rear = node;
    }
    else
    {            
        queue->rear->next = node;
        queue->rear = node;
    }
    ++queue->size;
}
 
int QueuePop(struct Queue* queue, UART_* UART)
{
    if (QueueEmpty(queue))
    {
        return 0;
    }
    struct Link* tmp = queue->front;
    *UART = queue->front->UART;
    queue->front = queue->front->next;
    free(tmp);
    --queue->size;

    return 1;
}
 
void QueueDestroy(struct Queue* queue)
{
    struct Link* tmp;
    while(queue->front)
    {
        tmp = queue->front;
        queue->front = queue->front->next;
        free(tmp);
    }
}