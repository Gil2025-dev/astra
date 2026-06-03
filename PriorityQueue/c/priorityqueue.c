
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct priority_queue {
    int *data;
    int size;
    int capacity;
};

void priority_heapify(struct priority_queue *pq, int i);

int priority_init(struct priority_queue *pq, int capacity) 
{
    if (capacity <= 0) {
        return (0); // Return 0 on invalid capacity
    }

    pq->size = 0;
    pq->capacity = capacity;
    pq->data = (int *)malloc(pq->capacity * sizeof(int));

    if (!pq->data) {
        return (0); // Return 0 on failure
    }
    return (1); // Return 1 on success
}

void priority_destroy(struct priority_queue *pq) 
{
    if (pq->data) {
        free(pq->data);
        pq->data = NULL;
    }

    pq->size = 0;
    pq->capacity = 0;
}

void priority_swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

bool priority_enqueue(struct priority_queue *pq, int value) 
{
    if (pq->size >= pq->capacity) {
        return false; // Queue is full
    }

    pq->data[pq->size++] = value;
    int index = pq->size - 1;

    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->data[parent] >= pq->data[index]) {
            break; // Heap property is satisfied
        }
        
        // Swap parent and child
        priority_swap(&pq->data[parent], &pq->data[index]);

        index = parent;
    }
    return true; // Enqueue successful
}

bool priority_dequeue(struct priority_queue *pq) 
{
    if (pq->size == 0) {
        return false; // Queue is empty
    }

    pq->data[0] = pq->data[--pq->size]; // Move last element to root
    priority_heapify(pq, 0); // Restore heap property

    return true; // Dequeue successful
}

int priority_peek(struct priority_queue *pq) 
{
    if (pq->size == 0) {
        return -1; // Queue is empty
    }

    return pq->data[0]; // Return the highest priority element
}

int priority_heap_depth(struct priority_queue *pq) 
{
    int size = pq->size;
    int depth = -1;
    while (size > 0) {
        size /= 2;
        depth++;
    }
    return depth;
}

// 힙 속성을 유지하도록 트리를 재구성
void priority_heapify(struct priority_queue *pq, int i) 
{
    int largest  = i;
    int left = 2 * i + 1;       // 좌측 자식 인덱스
    int right = 2 * i + 2;      // 우측 자식 인덱스

    // 좌측 자식이 가장 크면 largest를 좌측 자식으로
    if (left < pq->size && pq->data[left] > pq->data[largest]) {
        largest = left;
    }

    // 오른쪽 자식이 가장 크면 largest를 오른쪽 자식으로
    if (right < pq->size && pq->data[right] > pq->data[largest]) {
        largest  = right;
    }

    // largest가 i가 아니면 swap 후 재귀적으로 heapify
    if (largest != i) {
        priority_swap(&pq->data[i], &pq->data[largest]);
        priority_heapify(pq, largest);
    }
}


void priority_print(struct priority_queue *pq) 
{
    int depth = priority_heap_depth(pq);
    printf("Priority Queue (size: %d, depth: %d)\n", pq->size, depth);

    if (pq->size == 0) {
        printf("(empty)\n\n");
        return;
    }

    // Start = 2^i - 1, End = 2^(i+1) - 2
    // Level 0 : 0~0
    // Level 1 : 1~2, 
    // Level 2 : 3~6, 
    // Level 3 : 7~14
    for (int i = 0; i <= depth; i++) {
        int level_start = (1 << i) - 1; // 2^i - 1
        int level_end = (1 << (i + 1)) - 2; // 2^(i+1) - 2
        int indent = (1 << (depth - i + 1)) - 1;
        int gap = (1 << (depth - i + 2)) - 1;

        printf("Level %d: ", i);
        for (int s = 0; s < indent; s++) {
            printf(" ");
        }

        for (int j = level_start; j <= level_end && j < pq->size; j++) {
            printf("%d", pq->data[j]);
            for (int s = 0; s < gap; s++) {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("\n");
}


int main() 
{
    struct priority_queue pq;

    if (!priority_init(&pq, 10)) {
        printf("Failed to initialize priority queue\n");
        return -1;
    }

    priority_enqueue(&pq, 5);
    priority_enqueue(&pq, 3);
    priority_enqueue(&pq, 8);
    priority_enqueue(&pq, 1);
    priority_enqueue(&pq, 9);
    priority_enqueue(&pq, 12);
    priority_enqueue(&pq, 7);
    priority_enqueue(&pq, 4);
    printf("Priority Queue after enqueuing: ");
    priority_print(&pq);

    priority_dequeue(&pq);
    printf("Priority Queue after dequeuing: ");
    priority_print(&pq);

    priority_destroy(&pq);

    return 0;
}

/* end of file */
