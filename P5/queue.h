#ifndef QUEUE_H_   /* Include guard */
#define QUEUE_H_
#define MAX 20
struct QUEUE
{
    int intArray[MAX];
    int front;//0, -1, 0
    int rear;
    int itemCount;

};

int isEmpty(struct QUEUE q);
int isFull(struct QUEUE q);
int size(struct QUEUE q);
void insert(struct QUEUE *q, int data);
int removeData(struct QUEUE *q);

#endif // QUEUE_H_