#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char key[10];
    char name[300];
    struct Node* leftChild;
    struct Node* rightChild;
    int balance;
} Node;

typedef struct AVL {
    Node* root;
    int size;
} AVL;

AVL* newAvl(void);

Node* rotateLeft(Node* a);

Node* rotateRight(Node* a);

Node* bigRotateLeft(Node* a);

Node* bigRotateRight(Node* a);

Node* balance(Node* node);

Node* createNode(char* key, const char* name);

Node* insert(Node* node, char key[10], char name[300]);

Node* find(Node* node, const char* key);

Node* deleteNode(Node* node, const char* key);

void saveFile(Node* node, FILE* f);

void freeTree(Node* node);

void deleteAvl(AVL* avl);
