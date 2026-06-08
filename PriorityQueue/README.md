# PriorityQueue

이 PriorityQueue는 정수 값을 저장하는 PriorityQueue 자료구조를 C, C++로 구현한 예제입니다.

PriorityQueue는 삽입한 원소 중 우선순위가 가장 높은 것을 가장 먼저 꺼낼 수 있는 자료구조입니다. 이 구현은 배열 기반 최대 힙(Max-Heap)을 사용합니다. 최대 힙에서는 값이 클수록 우선순위가 높고, 항상 루트에 가장 큰 값이 위치합니다.

## 특징

- 정수 값 기반 최대 힙 구현
- 삽입(`enqueue`), 제거(`dequeue`), 최솟값 조회(`peek`) 지원
- 배열 기반 완전 이진 트리로 구현
- 고정 용량(`capacity`)으로 초기화
- C, C++ 구현 비교 가능

## 디렉터리 구성

```text
.
├── c/
│   ├── Makefile
│   └── priorityqueue.c
└── cpp/
    ├── Makefile
    └── priorityqueue.cpp
```

## 동작 개요

배열에 원소를 순서대로 채우면서 힙 속성을 유지합니다. 배열 인덱스 `i`의 자식은 `2i+1`(좌), `2i+2`(우)이고, 부모는 `(i-1)/2`입니다.

예를 들어 `5, 3, 8, 1, 9, 12, 7, 4`를 순서대로 삽입하면 다음과 같은 최대 힙이 만들어집니다.

```text
              12
           /       \
         8           9
       /   \       /   \
      4     3     5     7
     /
    1
```

배열로 표현하면:

```text
index:  0   1   2   3   4   5   6   7
data: [12,  8,  9,  4,  3,  5,  7,  1]
```

`dequeue`를 하면 루트(12)가 제거되고, 마지막 원소가 루트로 이동한 뒤 힙 속성을 복원합니다.

## 빌드 및 실행

### C

```bash
cd c
make
make run
```

메모리 누수 확인이 필요하면 다음 명령을 사용할 수 있습니다. `valgrind`가 설치되어 있으면 valgrind로 실행하고, 없으면 일반 실행으로 대체됩니다.

```bash
make leak
```

### C++

```bash
cd cpp
make
make run
```

메모리 누수 확인:

```bash
make leak
```

## 기본 사용 예

### C

```c
struct priority_queue pq;

if (!priority_init(&pq, 10)) {
    printf("Failed to initialize priority queue\n");
    return 1;
}

priority_enqueue(&pq, 5);
priority_enqueue(&pq, 3);
priority_enqueue(&pq, 8);

int top = priority_peek(&pq);
if (top != -1) {
    printf("top: %d\n", top);  // 8
}

priority_dequeue(&pq);
priority_print(&pq);
priority_destroy(&pq);
```

### C++

```cpp
priorityqueue pq(10);

pq.enqueue(5);
pq.enqueue(3);
pq.enqueue(8);

int top = pq.peek();
if (top != -1) {
    printf("top: %d\n", top);  // 8
}

pq.dequeue();
pq.print();
```

## 주요 파라미터 및 반환값

| 항목 | 의미 |
| --- | --- |
| `capacity` | 큐가 저장할 수 있는 최대 원소 수 |
| `size` | 현재 저장된 원소 수 |
| `peek` 반환값 | 가장 높은 우선순위 원소, 비어 있으면 `-1` |
| `enqueue` 반환값 | 성공 시 `true`, 큐가 가득 찼을 때 `false` |
| `dequeue` 반환값 | 성공 시 `true`, 큐가 비어 있을 때 `false` |

## 시간 복잡도

| 연산 | 시간 복잡도 |
| --- | --- |
| 삽입(`enqueue`) | `O(log n)` |
| 제거(`dequeue`) | `O(log n)` |
| 조회(`peek`) | `O(1)` |
| 공간 | `O(n)` |

## 주의 사항

- 현재 구현은 고정 용량으로, 가득 차면 `enqueue`가 실패합니다.
- `peek`은 큐가 비어 있으면 `-1`을 반환합니다. 저장된 값이 `-1`인 경우와 구별할 수 없으므로 `size`로 먼저 확인하는 것이 권장됩니다.
- C/C++ 구현은 동적 메모리를 사용하므로 종료 시 반드시 해제 경로가 필요합니다.
