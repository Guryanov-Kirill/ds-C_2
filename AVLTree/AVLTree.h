#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// узел дерева, храним ключ, имя и указатели на детей
// balance это разница высот правого и левого поддерева
typedef struct Node {
    char* key; // ключ по которому ищем
    char* name; // само значение
    struct Node* leftChild;
    struct Node* rightChild;
    int balance; // находится в пределах (-2 < 0 < 2)
} Node;

// само дерево, root - корень, size кол-во элементов
typedef struct AVL {
    Node* root;
    int size;
} AVL;

// создаёт пустое дерево
AVL* newAvl(void);

// Повороты нужны чтобы дерево оставалось сбалансированным
//  после вставки или удаления

// малый левый поворот вокруг узла a
Node* rotateLeft(Node* a);

// малый правый поворот вокруг узла a
Node* rotateRight(Node* a);

// большой левый поворот (право-левый случай)
Node* bigRotateLeft(Node* a);

// большой правый поворот (лево-правый случай)
Node* bigRotateRight(Node* a);

// балансирует узел если balance вышел за пределы(-1, 0, 1)
Node* balance(Node* node);

// выделяет память под новый узел и заполняет поля
Node* createNode(char* key, const char* name);

// вставка в поддерево node, возвращает новый корень поддерева
Node* insert(Node* node, char* key, char* name);

// поиск по ключу, вернёт NULL если не нашли
Node* find(Node* node, const char* key);

// удаление узла по ключу, возвращает новый корень
Node* deleteNode(Node* node, const char* key);

// записывает дерево в файл (обход inorder чтобы было отсортировано)
void saveFile(Node* node, FILE* f);

// рекурсивно освобождает всё дерево
void freeTree(Node* node);

// удаляет AVL структуру вместе с деревом
void deleteAvl(AVL* avl);
