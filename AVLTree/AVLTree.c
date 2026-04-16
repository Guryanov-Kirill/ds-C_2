#include "AVLTree.h"

AVL* newAvl(void)
{
    AVL* avl = (AVL*)malloc(sizeof(AVL));
    if (avl == NULL) {
        printf("Allocation error\n");
        return NULL;
    }
    avl->root = NULL;
    avl->size = 0;
    return avl;
}

Node* rotateLeft(Node* a)
{
    Node* b = a->rightChild;
    Node* c = b->leftChild;
    b->leftChild = a;
    a->rightChild = c;
    a->balance = 0;
    b->balance = 0;
    return b;
}

Node* rotateRight(Node* a)
{
    Node* b = a->leftChild;
    Node* c = b->rightChild;
    b->rightChild = a;
    a->leftChild = c;
    a->balance = 0;
    b->balance = 0;
    return b;
}

Node* bigRotateLeft(Node* a)
{
    Node* b = a->rightChild;
    Node* c = b->leftChild;
    Node* d = c->rightChild;
    c->rightChild = b;
    b->leftChild = d;

    d = c->leftChild;
    c->leftChild = a;
    a->rightChild = d;

    if (c->balance == -1) {
        a->balance = 0;
        b->balance = 1;
    } else if (c->balance == 1) {
        a->balance = -1;
        b->balance = 0;
    } else {
        a->balance = 0;
        b->balance = 0;
    }
    c->balance = 0;
    return c;
}

Node* bigRotateRight(Node* a)
{
    Node* b = a->leftChild;
    Node* c = b->rightChild;
    Node* d = c->leftChild;
    c->leftChild = b;
    b->rightChild = d;

    d = c->rightChild;
    c->rightChild = a;
    a->leftChild = d;

    if (c->balance == -1) {
        a->balance = 1;
        b->balance = 0;
    } else if (c->balance == 1) {
        a->balance = 0;
        b->balance = -1;
    } else {
        a->balance = 0;
        b->balance = 0;
    }
    c->balance = 0;
    return c;
}

Node* balance(Node* node)
{
    if (node == NULL) {
        return NULL;
    }
    if (node->balance == 2) {
        if (node->rightChild == NULL) {
            node->balance = 0;
            return node;
        }
        if (node->rightChild->balance >= 0) {
            return rotateLeft(node);
        }
        return bigRotateLeft(node);
    }
    if (node->balance == -2) {
        if (node->leftChild == NULL) {
            node->balance = 0;
            return node;
        }
        if (node->leftChild->balance <= 0) {
            return rotateRight(node);
        }
        return bigRotateRight(node);
    }
    return node;
}

Node* createNode(char* key, const char* name)
{
    Node* node = calloc(1, sizeof(Node));
    if (node == NULL) {
        printf("Allocation error\n");
        return NULL;
    }
    node->key = strdup(key);
    node->name = strdup(name);
    node->balance = 0;
    return node;
}

Node* insert(Node* node, char* key, char* name)
{
    if (node == NULL) {
        return createNode(key, name);
    }
    int current = strcmp(key, node->key);
    if (current < 0) {
        node->leftChild = insert(node->leftChild, key, name);
        --node->balance;
    } else if (current > 0) {
        node->rightChild = insert(node->rightChild, key, name);
        ++node->balance;
    } else {
        return node;
    }
    return balance(node);
}

Node* find(Node* node, const char* key)
{
    if (node == NULL) {
        return NULL;
    }
    int current = strcmp(key, node->key);
    if (current == 0) {
        return node;
    } else if (current < 0) {
        return find(node->leftChild, key);
    } else {
        return find(node->rightChild, key);
    }
}

Node* deleteNode(Node* node, const char* key)
{
    if (node == NULL) {
        return NULL;
    }
    int cmp = strcmp(key, node->key);
    if (cmp < 0) {
        node->leftChild = deleteNode(node->leftChild, key);
        node->balance++;
    } else if (cmp > 0) {
        node->rightChild = deleteNode(node->rightChild, key);
        node->balance--;
    } else {
        Node* left = node->leftChild;
        Node* right = node->rightChild;
        int currentBalance = node->balance;
        if (right == NULL) {
            free(node);
            return left;
        }
        Node* rightMin = right;
        while (rightMin->leftChild != NULL) {
            rightMin = rightMin->leftChild;
        }
        Node* current = createNode(rightMin->key, rightMin->name);
        current->rightChild = deleteNode(right, current->key);
        current->leftChild = left;
        current->balance = currentBalance - 1;
        free(node);
        return balance(current);
    }
    return balance(node);
}

void saveFile(Node* node, FILE* f)
{
    if (node == NULL) {
        return;
    }
    saveFile(node->leftChild, f);
    fprintf(f, "%s:%s\n", node->key, node->name);
    saveFile(node->rightChild, f);
}

void freeTree(Node* node)
{
    if (node == NULL) {
        return;
    }
    freeTree(node->leftChild);
    freeTree(node->rightChild);
    free(node->key);
    free(node->name);
    free(node);
}

void deleteAvl(AVL* avl)
{
    if (avl == NULL) {
        return;
    }
    freeTree(avl->root);
    free(avl);
}
