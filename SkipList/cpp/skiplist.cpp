#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector> 
#include <iostream> 

class Node {
private:
    int key;
    // int value;
    std::vector<Node*> next;  // 각 레벨에서 다음 노드를 가리키는 포인터 배열
public:
    Node(int key, int level) : key(key), next(level, nullptr) {}
    int getKey() const { return key; }
    std::vector<Node*>& getNext() { return next; }
};


class Skiplist
{
private:
    Node *head;
    float p;              // probability for random level generation
    int max_level;
    int current_level;

    int random_level();
public:
    Skiplist(float p, int max_level);
    ~Skiplist();
    
    // ★ 복사 방지 코드 (Copy Prevention)
    // 1. 복사 생성자 차단: Skiplist list2(list1); -> 컴파일 에러!
    Skiplist(const Skiplist&) = delete;
    // 2. 복사 대입 연산자 차단: list2 = list1; -> 컴파일 에러!
    Skiplist& operator=(const Skiplist&) = delete;

    bool insert(int key);
    Node* search(int key);
    void remove(int key);
    void print();
};

int Skiplist::random_level()
{
    int level = 0;
    while (((float)rand() / RAND_MAX) < p && level < max_level - 1) {
        level++;
    }
    return level;
}

Skiplist::Skiplist(float p, int max_level) : p(p), max_level(max_level), current_level(0) {
    if (p <= 0 || p >= 1) {
        throw std::invalid_argument("p must be between 0 and 1");
    }
    if (max_level <= 0) {
        throw std::invalid_argument("max_level must be positive");
    }

    head = new Node(-1, max_level);  // 헤드 노드는 키 -1로 초기화
}

Skiplist::~Skiplist()
{
    Node *current = head;
    while (current != nullptr) {
        Node *next = current->getNext()[0];
        delete current;
        current = next;
    }
}

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


bool Skiplist::insert(int key)
{
    std::vector<Node*> update(max_level, nullptr);  // 각 레벨에서 삽입 위치를 추적하는 배열
    Node *current = head;

    // update 배열을 사용하여 각 레벨에서 삽입 위치를 추적
    for (int i = current_level; i >= 0; i--) {
        while (current->getNext()[i] != nullptr && current->getNext()[i]->getKey() < key) {
            current = current->getNext()[i];
        }
        update[i] = current;
    }

    // 레벨 0에서 다음 노드를 확인하여 중복 키가 있는지 검사
    current = current->getNext()[0];
    if (current != nullptr && current->getKey() == key) {
        return false;  // 중복 키는 허용하지 않음
    }

    // 새로운 노드의 레벨을 결정 (0-based: 0..max_level-1)
    int new_level = random_level();
    if (new_level > current_level) {
        for (int i = current_level + 1; i <= new_level; i++) {
            update[i] = head;  // 새로운 레벨에서는 헤드 노드를 업데이트
        }
        current_level = new_level;  // Skiplist의 현재 레벨 업데이트
    }

    // 새로운 노드 생성
    Node *new_node = new Node(key, new_level + 1);

    // update 배열을 사용하여 각 레벨에서 새로운 노드를 삽입 (0..new_level 포함)
    for (int i = 0; i <= new_level; i++) {
        new_node->getNext()[i] = update[i]->getNext()[i];
        update[i]->getNext()[i] = new_node;
    }

    return true;
}

Node* Skiplist::search(int key)
{
    Node *current = head;

    for (int i = current_level; i >= 0; i--) {
        while (current->getNext()[i] != nullptr && current->getNext()[i]->getKey() < key) {
            current = current->getNext()[i];
        }
    }

    current = current->getNext()[0];
    if (current != nullptr && current->getKey() == key) {
        return current;  // 키를 찾음
    }

    return nullptr;  // 키를 찾지 못함
}

void Skiplist::remove(int key)
{
    std::vector<Node*> update(max_level, nullptr);  // 각 레벨에서 삭제 위치를 추적하는 배열
    Node *current = head;

    // update 배열을 사용하여 각 레벨에서 삭제 위치를 추적
    for (int i = current_level; i >= 0; i--) {
        while (current->getNext()[i] != nullptr && current->getNext()[i]->getKey() < key) {
            current = current->getNext()[i];
        }
        update[i] = current;
    }

    // 레벨 0에서 다음 노드를 확인하여 삭제할 노드가 있는지 검사
    current = current->getNext()[0];
    if (current == nullptr || current->getKey() != key) {
        return;  // 삭제할 키가 없음
    }

    // update 배열을 사용하여 각 레벨에서 노드를 삭제 (0..current의 레벨 포함)
    for (int i = 0; i <= current_level; i++) {
        if (update[i]->getNext()[i] != current) {
            break;  // 현재 레벨에서 더 이상 노드가 없으면 중단
        }
        update[i]->getNext()[i] = current->getNext()[i];
    }

    delete current;  // 삭제된 노드 메모리 해제

    // Skiplist의 현재 레벨이 비어 있으면 레벨을 낮춤
    while (current_level > 0 && head->getNext()[current_level] == nullptr) {
        current_level--;
    }
}

void Skiplist::print()
{
    for (int i = current_level; i >= 0; i--) {
        Node *current = head->getNext()[i];
        std::cout << "Level " << i << ": ";
        while (current != nullptr) {
            std::cout << current->getKey() << " ";
            current = current->getNext()[i];
        }
        std::cout << std::endl;
    }
}

int main()
{
    Skiplist list = Skiplist(0.5, 6);  // 확률 p = 0.5, 최대 레벨 = 6

    srand(time(NULL));  // Initialize random seed
    list.insert(3);
    list.insert(9);
    list.insert(7);
    list.insert(6);
    // list.insert(12);
    // list.insert(19);

    list.print();

    return 0;
}
