#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct node_t {
    int key;
    // int value;
    struct node_t **next;
};

struct skiplist_t {
    struct node_t *head;
    float p;              // probability for random level generation
    int max_level;
    int level;
};

// Level 1: [head] ──────────────────→ 7 ──→ 9 ──→ NULL
// Level 0: [head] ──→ 3 ──→ 6 ──→ 7 ──→ 9 ──→ NULL
// - Level1 
// head->next[1]->next[1] = node(9)
// head->next[1]->next[1]->next[1] = NULL
// - Level 0
// head->next[0] = node(3)
// head->next[0]->next[0] = node(6)
// head->next[0]->next[0]->next[0] = node(7)     ← head->next[1]과 동일 노드
// head->next[0]->next[0]->next[0]->next[0] = node(9)     ← head->next[1]->next[1]과 동일 노드
// head->next[0]->next[0]->next[0]->next[0]->next[0] = NULL


struct node_t *create_node(int key, int level)
{
    struct node_t *node = (struct node_t *)malloc(sizeof(struct node_t));

    if (node == NULL) {
        return (NULL);
    }

    node->key = key;
    node->next = (struct node_t **)malloc(sizeof(struct node_t *) * level);
    if (node->next == NULL) {
        free(node);
        return (NULL);
    }

    for (int i = 0; i < level; i++) {
        node->next[i] = NULL;
    }

    return (node);
}

void node_free(struct node_t *node)
{
    if (node != NULL) {
        free(node->next);
        free(node);
    }
}   

// 0..max_level-1 범위의 레벨 인덱스를 반환하는 함수 (0-based)
int random_level(struct skiplist_t *list)
{
    int level = 0;

    // (float)rand() / RAND_MAX 는 0.0에서 1.0 사이의 난수를 생성
    while ((rand() / (float)RAND_MAX) < list->p && level < list->max_level - 1) {
        level++;
    }

    return (level);
}

int skiplist_init(struct skiplist_t *list, int max_level, float p)
{
    if (p <= 0 || p >= 1) {
        return (0);  // p는 0과 1 사이의 값이어야 함
    }
    if (max_level <= 0) {
        return (0);  // max_level은 양수여야 함
    }

    list->head = create_node(-1, max_level);  // 헤드 노드는 키 -1로 초기화
    if (list->head == NULL) {
        return (0);
    }

    list->p = p;

    list->max_level = max_level;
    list->level = 0;  // 초기 레벨은 0

    return (1);
}

void skiplist_destroy(struct skiplist_t *list)
{
    struct node_t *current = list->head;

    while (current != NULL) {
        struct node_t *next = current->next[0];
        node_free(current);
        current = next;
    }
}


int skiplist_insert(struct skiplist_t *list, int key)
{
    struct node_t **update;  // [list->max_level];  // 각 레벨에서 삽입 위치를 추적하는 배열
    struct node_t *current = list->head;

    update = (struct node_t **)malloc(sizeof(struct node_t *) * list->max_level);
    if (update == NULL) {
        return (0);
    }

    // update 배열을 사용하여 각 레벨에서 삽입 위치를 추적
    for (int i = list->level; i >= 0; i--) {
        while (current->next[i] != NULL && current->next[i]->key < key) {
            current = current->next[i];
        }
        update[i] = current;
    }

    // 레벨 0에서 다음 노드를 확인하여 중복 키가 있는지 검사
    current = current->next[0];
    if (current != NULL && current->key == key) {
        free(update);
        return (0);  // 중복 키는 허용하지 않음
    }

    // 새로운 노드의 레벨을 결정 (0-based: 0..max_level-1)
    int new_level = random_level(list);
    if (new_level > list->level) {
        for (int i = list->level + 1; i <= new_level; i++) {
            update[i] = list->head;  // 새로운 레벨에서는 헤드 노드를 업데이트
        }
    }

    // 새로운 노드 생성
    struct node_t *new_node = create_node(key, new_level+1);
    if (new_node == NULL) {
        free(update);
        return (0);
    }

    // update 배열을 사용하여 각 레벨에서 새로운 노드를 삽입 (0..new_level 포함)
    for (int i = 0; i <= new_level; i++) {
        new_node->next[i] = update[i]->next[i];
        update[i]->next[i] = new_node;
    }

    // skiplist의 현재 레벨보다 새로운 노드의 레벨이 높으면 skiplist의 레벨을 업데이트
    if (new_level > list->level) {
        printf("Updating skiplist level from %d to %d\n", list->level, new_level);
        list->level = new_level;
    }

    free(update);
    return (1);
}

struct node_t *skiplist_search(struct skiplist_t *list, int key)
{
    struct node_t *current = list->head;

    for (int i = list->level; i >= 0; i--) {
        while (current->next[i] != NULL
               && current->next[i]->key < key) {
            current = current->next[i];
        }
    }

    current = current->next[0];
    if (current != NULL && current->key == key) {
        return (current);  // 키를 찾음
    }

    return (NULL);  // 키를 찾지 못함
}

void skiplist_delete(struct skiplist_t *list, int key)
{
    struct node_t **update;  // [list->max_level];  // 각 레벨에서 삭제 위치를 추적하는 배열
    struct node_t *current = list->head;

    update = (struct node_t **)malloc(sizeof(struct node_t *) * list->max_level);
    if (update == NULL) {
        return;
    }

    // update 배열을 사용하여 각 레벨에서 삭제 위치를 추적
    for (int i = list->level; i >= 0; i--) {
        while (current->next[i] != NULL && current->next[i]->key < key) {
            current = current->next[i];
        }
        update[i] = current;
    }

    // 레벨 0에서 다음 노드를 확인하여 삭제할 노드가 있는지 검사
    current = current->next[0];
    if (current == NULL || current->key != key) {
        free(update);
        return;  // 삭제할 키가 없음
    }

    // update 배열을 사용하여 각 레벨에서 노드를 삭제 (0..current의 레벨 포함)
    for (int i = 0; i <= list->level; i++) {
        if (update[i]->next[i] != current) {
            break;  // 현재 레벨에서 더 이상 노드가 없으면 중단
        }
        update[i]->next[i] = current->next[i];
    }

    node_free(current);  // 삭제된 노드 메모리 해제

    // skiplist의 현재 레벨이 비어 있으면 레벨을 낮춤
    while (list->level > 0 && list->head->next[list->level] == NULL) {
        list->level--;
    }

    free(update);
}

void skiplist_print(struct skiplist_t *list)
{
    // level 0의 모든 키와 출력 폭을 수집
    int count = 0;
    struct node_t *node = list->head->next[0];
    while (node != NULL) {
        count++;
        node = node->next[0];
    }

    if (count == 0) return;

    int *keys = (int *)malloc(sizeof(int) * count);
    int *widths = (int *)malloc(sizeof(int) * count);
    node = list->head->next[0];
    for (int j = 0; j < count; j++) {
        keys[j] = node->key;
        widths[j] = snprintf(NULL, 0, "%d", node->key);
        node = node->next[0];
    }

    // 각 레벨을 컬럼 정렬하여 출력
    for (int i = list->level; i >= 0; i--) {
        printf("Level %d: ", i);
        struct node_t *current = list->head->next[i];
        for (int j = 0; j < count; j++) {
            if (current != NULL && current->key == keys[j]) {
                printf("%-*d ", widths[j], current->key);
                current = current->next[i];
            } else {
                // 빈 칸: 해당 폭만큼 공백 출력
                printf("%*s ", widths[j], "");
            }
        }
        printf("\n");
    }

    free(keys);
    free(widths);
}

int main()
{
    struct skiplist_t list;

    srand(time(NULL));  // Initialize random seed

    if (!skiplist_init(&list, 6, 0.5)) {
        printf("Failed to initialize skiplist\n");
        return 1;
    }

    skiplist_insert(&list, 3);
    skiplist_insert(&list, 7);
    skiplist_insert(&list, 9);
    skiplist_insert(&list, 6);
    // skiplist_insert(&list, 12);
    // skiplist_insert(&list, 19);

    skiplist_print(&list);

    skiplist_destroy(&list);

    return 0;
}
