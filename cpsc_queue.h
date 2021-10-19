typedef struct {
    int front;
    int num;
    int capacity;
    int* arr;
} Queue;

// =====================================================================================================================

void initializeQueue(Queue* dataQueue, int queueSize)
{
    dataQueue->front = 0;
    dataQueue->num = 0;
    dataQueue->capacity = queueSize;
    dataQueue->arr = (int*) malloc(dataQueue->capacity * sizeof(int));
}

// =====================================================================================================================

int isEmpty(Queue* q)
{
    if (q->num == 0)
        return TRUE;
    else
        return FALSE;
}

// =====================================================================================================================

int isFull(Queue* q) {
    if (q->num == q->capacity)
        return TRUE;
    else
        return FALSE;
}

// =====================================================================================================================

int enQueue(Queue* q, int val)
{
    if (isFull(q))
        return FALSE;
    else
    {
        q->arr[(q->front + q->num) % q->capacity] = val;
        q->num++;
        return TRUE;
    }
}

// =====================================================================================================================

int deQueue(Queue* q) {
    if (isEmpty(q))
        return FALSE;
    else
    {
        int deQueuedItem;
        deQueuedItem = q->arr[q->front];
        q->arr[q->front] = -1;
        q->front = (q->front + 1) % q->capacity;
        q->num--;
        return deQueuedItem;
    }
}

// =====================================================================================================================
