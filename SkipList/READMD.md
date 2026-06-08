# SkipList

이 SkipList는 정수 key를 저장하는 SkipList 자료구조를 C, C++로 구현한 예제입니다.

SkipList는 정렬된 연결 리스트에 여러 단계의 "빠른 길" 레벨을 추가한 확률적 자료구조입니다. 낮은 레벨에는 모든 노드가 있고, 높은 레벨에는 일부 노드만 올라갑니다. 탐색할 때는 가장 높은 레벨에서 오른쪽으로 이동하다가 더 진행할 수 없으면 한 단계 내려가는 방식으로 목표 key에 접근합니다.

## 특징

- 정수 key 기반 정렬 저장
- 중복 key 삽입 방지
- 삽입, 탐색, 삭제 지원
- 확률 `p`에 따라 노드 높이 결정
- 최대 레벨 `max_level`로 높이 제한
- C, C++ 구현 비교 가능

## 디렉터리 구성

```text
.
├── c/
│   ├── Makefile
│   └── skiplist.c
└── cpp/
    ├── Makefile
    └── skiplist.cpp
```

## 동작 개요

예를 들어 `3, 6, 7, 9`가 저장되어 있고 `7, 9`가 상위 레벨에도 올라간 경우 구조는 다음처럼 볼 수 있습니다.

```text
Level 1: [head] --------> 7 -> 9 -> NULL
Level 0: [head] -> 3 -> 6 -> 7 -> 9 -> NULL
```

모든 노드는 level 0에 연결되고, 랜덤하게 선택된 일부 노드는 더 높은 레벨에도 연결됩니다. 이 구조 덕분에 일반 연결 리스트처럼 처음부터 끝까지 순회하지 않고도 key를 빠르게 찾을 수 있습니다.

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
struct skiplist_t list;

srand(time(NULL));

if (!skiplist_init(&list, 6, 0.5)) {
    printf("Failed to initialize skiplist\n");
    return 1;
}

skiplist_insert(&list, 3);
skiplist_insert(&list, 7);
skiplist_insert(&list, 9);
skiplist_insert(&list, 6);

struct node_t *found = skiplist_search(&list, 7);
if (found != NULL) {
    printf("found: %d\n", found->key);
}

skiplist_delete(&list, 7);
skiplist_print(&list);
skiplist_destroy(&list);
```

### C++

```cpp
Skiplist list(0.5, 6);

srand(time(NULL));

list.insert(3);
list.insert(7);
list.insert(9);
list.insert(6);

Node *found = list.search(7);
if (found != nullptr) {
    std::cout << "found: " << found->getKey() << std::endl;
}

list.remove(7);
list.print();
```

## 주요 파라미터

| 이름 | 의미 |
| --- | --- |
| `max_level` | SkipList가 가질 수 있는 최대 레벨 수 |
| `p` | 노드가 한 단계 더 높은 레벨로 올라갈 확률 |
| `level` 또는 `current_level` | 현재 SkipList에서 사용 중인 가장 높은 레벨 인덱스 |

`p`는 `0 < p < 1` 범위여야 합니다. 일반적으로 `0.5`를 사용하면 각 레벨의 노드 수가 평균적으로 절반씩 줄어드는 형태가 됩니다.

## 시간 복잡도

| 연산 | 평균 |
| --- | --- |
| 탐색 | `O(log n)` |
| 삽입 | `O(log n)` |
| 삭제 | `O(log n)` |

최악의 경우 모든 노드가 낮은 레벨에만 몰릴 수 있으므로 `O(n)`이 될 수 있지만, 확률적으로는 균형 잡힌 계층 구조를 기대할 수 있습니다.

## 주의 사항

- 현재 구현은 key만 저장하며 별도의 value는 저장하지 않습니다.
- 중복 key는 삽입되지 않습니다.
- C/C++ 구현은 동적 메모리를 사용하므로 종료 시 반드시 해제 경로가 필요합니다.
