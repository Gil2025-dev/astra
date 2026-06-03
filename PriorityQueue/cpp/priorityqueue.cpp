
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdexcept>

class priorityqueue
{
private:
    int *data;
    int size;
    int capacity;

    void swap(int *a, int *b);

public:
    priorityqueue(int capacity);
    ~priorityqueue();
    bool enqueue(int value);
    void heapify(int i);
    bool dequeue();
    int peek();
    int depth();
    void print();
};

void priorityqueue::swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


priorityqueue::priorityqueue(int capacity)
{
    if (capacity <= 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }

    size = 0;
    this->capacity = capacity;
    data = new int[capacity];

    if (!data) {
        throw std::bad_alloc(); // Throw exception on allocation failure
    }
}

priorityqueue::~priorityqueue()
{
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
}


bool priorityqueue::enqueue(int value) 
{
    if (size >= capacity) {
        return false; // Queue is full
    }

    data[size++] = value;
    int i = size - 1;

    while (i > 0 && data[(i - 1) / 2] < data[i]) {
        // Swap parent and child
        swap(&data[(i - 1) / 2], &data[i]);
        i = (i - 1) / 2;
    }

    return true; // Enqueue successful
}

bool priorityqueue::dequeue() 
{
    if (size == 0) {
        return false; // Queue is empty
    }

    data[0] = data[--size]; // Move last element to root    
    heapify(0);

    return true; // Dequeue successful
}

int priorityqueue::peek() 
{
    if (size == 0) {
        return -1; // Queue is empty
    }

    return data[0]; // Return the highest priority element
}

int priorityqueue::depth() 
{
    int d = -1;
    int n = size;

    while (n > 0) {
        n /= 2;
        d++;
    }
    return d; // Depth is one less than the number of levels
}

// 힙 속성을 유지하도록 트리를 재구성
void priorityqueue::heapify(int i) 
{
    int largest  = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // 좌측 자식이 가장 크면 largest를 좌측 자식으로
    if (left < size && data[left] > data[largest]) {
        largest  = left;
    }

    // 오른쪽 자식이 가장 크면 largest를 오른쪽 자식으로
    if (right < size && data[right] > data[largest]) {
        largest  = right;
    }

    // largest가 i가 아니면 swap 후 재귀적으로 heapify
    if (largest != i) {
        swap(&data[i], &data[largest]);
        heapify(largest);
    }
}

void priorityqueue::print() 
{
int d = depth();
    printf("Priority Queue (size: %d, depth: %d)\n", size, d);

    if (size == 0) {
        printf("(empty)\n\n");
        return;
    }

    // Start = 2^i - 1, End = 2^(i+1) - 2
    // Level 0 : 0~0
    // Level 1 : 1~2, 
    // Level 2 : 3~6, 
    // Level 3 : 7~14

    for (int i = 0; i <= d; i++) {
        int level_start = (1 << i) - 1; // 2^i - 1
        int level_end = (1 << (i + 1)) - 2; // 2^(i+1) - 2
        int indent = (1 << (d - i + 1)) - 1; // 2^(d-i+1) - 1
        int gap = (1 << (d - i + 2)) - 1; // 2^(d-i+2) - 1

        printf("Level %d: ", i);
        for (int s = 0; s < indent; s++) {
            printf(" ");
        }

        for (int j = level_start; j <= level_end && j < size; j++) {
            printf("%d", data[j]);
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
    priorityqueue pq(10);

    pq.enqueue(5);
    pq.enqueue(3);
    pq.enqueue(8);
    pq.enqueue(1);
    pq.enqueue(9);
    pq.enqueue(12);
    pq.enqueue(7);
    pq.enqueue(4);
    printf("Priority Queue after enqueuing: ");
    pq.print();

    pq.dequeue();
    printf("Priority Queue after dequeuing: ");
    pq.print();
    
    return 0;
}
