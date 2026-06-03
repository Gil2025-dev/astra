# Astra

C와 C++로 구현한 기본 자료구조 예제 프로젝트입니다. 현재는 우선순위 큐와 스킵 리스트 구현을 포함합니다.

## 프로젝트 구조

```text
.
├── PriorityQueue
│   ├── c
│   │   ├── Makefile
│   │   └── priorityqueue.c
│   └── cpp
│       ├── Makefile
│       └── priorityqueue.cpp
└── SkipList
    ├── c
    │   ├── Makefile
    │   └── skiplist.c
    └── cpp
        ├── Makefile
        └── skiplist.cpp
```

## 구현 내용

### PriorityQueue

최대 힙 기반 우선순위 큐 구현입니다.

- `enqueue`: 값을 삽입한 뒤 부모 노드와 비교하며 위로 올려 힙 속성을 유지합니다.
- `dequeue`: 루트 값을 제거하고 마지막 원소를 루트로 이동한 뒤 `heapify`로 힙을 복구합니다.
- `peek`: 현재 가장 높은 우선순위 값을 반환합니다.
- `print`: 힙의 레벨별 구조를 출력합니다.

### SkipList

확률적 레벨을 사용하는 정렬 자료구조 구현입니다.

- `insert`: 중복 키를 허용하지 않고 정렬 위치에 새 노드를 삽입합니다.
- `search`: 상위 레벨부터 탐색 범위를 줄이며 키를 찾습니다.
- `delete` 또는 `remove`: 지정한 키의 노드를 삭제합니다.
- `print`: 현재 스킵 리스트의 레벨별 노드를 출력합니다.

## 요구 사항

- Linux 또는 WSL2 환경
- `make`
- C 컴파일러: `gcc`
- C++ 컴파일러: `g++`
- 선택 사항: `valgrind`

## 빌드 및 실행

각 구현 디렉터리에는 동일한 형태의 Makefile이 있습니다.

```bash
make        # 빌드
make run    # 빌드 후 실행
make leak   # valgrind가 있으면 메모리 검사 실행
make clean  # 빌드 산출물 삭제
```

### PriorityQueue C

```bash
cd PriorityQueue/c
make run
```

### PriorityQueue C++

```bash
cd PriorityQueue/cpp
make run
```

### SkipList C

```bash
cd SkipList/c
make run
```

### SkipList C++

```bash
cd SkipList/cpp
make run
```

## 예시

우선순위 큐 예제는 `5, 3, 8, 1, 9, 12, 7, 4`를 삽입한 뒤 힙 구조를 출력하고, 한 번 삭제한 뒤 다시 출력합니다.

스킵 리스트 예제는 `3, 7, 9, 6`을 삽입한 뒤 레벨별 노드 구성을 출력합니다. 스킵 리스트는 난수 기반으로 노드 레벨을 정하므로 실행할 때마다 출력 레벨이 달라질 수 있습니다.

## 정리

이 프로젝트는 자료구조의 핵심 동작을 C와 C++ 양쪽에서 비교하며 학습할 수 있도록 구성되어 있습니다.  
각 소스 파일은 독립 실행 가능한 예제를 포함하므로, 구현을 수정한 뒤 바로 `make run`으로 결과를 확인할 수 있습니다.
