#include "AVLTree.h"
#include <stdio.h>
#include <string.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

bool testAvlInsert1(void)
{
    AVL* avl = newAvl();
    avl->root = insert(avl->root, "SVO", "Sheremetyevo");
    avl->root = insert(avl->root, "DME", "Domodedovo");
    avl->root = insert(avl->root, "VKO", "Vnukovo");

    if (strcmp(avl->root->key, "SVO") != 0) {
        deleteAvl(avl);
        return false;
    }
    if (strcmp(avl->root->leftChild->key, "DME") != 0 || strcmp(avl->root->rightChild->key, "VKO") != 0) {
        deleteAvl(avl);
        return false;
    }

    deleteAvl(avl);
    return true;
}

bool testAvlBalanceRR(void)
{
    AVL* avl = newAvl();
    avl->root = insert(avl->root, "AAA", "A");
    avl->root = insert(avl->root, "BBB", "B");
    avl->root = insert(avl->root, "CCC", "C");

    bool res = (strcmp(avl->root->key, "BBB") == 0);

    deleteAvl(avl);
    return res;
}

bool testAvlBalanceLL(void)
{
    AVL* avl = newAvl();
    avl->root = insert(avl->root, "CCC", "C");
    avl->root = insert(avl->root, "BBB", "B");
    avl->root = insert(avl->root, "AAA", "A");

    bool res = (strcmp(avl->root->key, "BBB") == 0);

    deleteAvl(avl);
    return res;
}

bool testAvlBalanceLR(void)
{
    AVL* avl = newAvl();
    avl->root = insert(avl->root, "CCC", "C");
    avl->root = insert(avl->root, "AAA", "A");
    avl->root = insert(avl->root, "BBB", "B");

    bool res = (strcmp(avl->root->key, "BBB") == 0);

    deleteAvl(avl);
    return res;
}

bool testAvlFind(void)
{
    AVL* avl = newAvl();
    avl->root = insert(avl->root, "JFK", "New York");
    avl->root = insert(avl->root, "LED", "Saint Petersburg");

    Node* n1 = find(avl->root, "JFK");
    Node* n2 = find(avl->root, "LAX");

    bool res = (n1 != NULL && strcmp(n1->name, "New York") == 0 && n2 == NULL);

    deleteAvl(avl);
    return res;
}

bool testAvlDeleteLeaf(void)
{
    AVL* avl = newAvl();
    avl->root = insert(avl->root, "BBB", "B");
    avl->root = insert(avl->root, "AAA", "A");

    avl->root = deleteNode(avl->root, "AAA");

    bool res = (find(avl->root, "AAA") == NULL && avl->root->leftChild == NULL);

    deleteAvl(avl);
    return res;
}

bool testAvlDeleteTwoChildren(void)
{
    AVL* avl = newAvl();
    avl->root = insert(avl->root, "B", "B");
    avl->root = insert(avl->root, "A", "A");
    avl->root = insert(avl->root, "D", "D");
    avl->root = insert(avl->root, "C", "C");
    avl->root = insert(avl->root, "E", "E");
    avl->root = deleteNode(avl->root, "B");
    bool res = (find(avl->root, "B") == NULL && find(avl->root, "C") != NULL && find(avl->root, "D") != NULL);
    deleteAvl(avl);
    return res;
}

int main(int argc, char** argv)
{
    bool testMode = false;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--test") == 0) {
            testMode = true;
            break;
        }
    }

    if (testMode) {
        const int TEST_NUM = 8;
        bool good = true;
        bool (*tests[])(void) = {
            &testAvlInsert1,
            &testAvlBalanceRR,
            &testAvlBalanceLL,
            &testAvlBalanceLR,
            &testAvlFind,
            &testAvlDeleteLeaf,
            &testAvlDeleteTwoChildren,
        };

        for (int testNum = 0; testNum < TEST_NUM; ++testNum) {
            if (tests[testNum]()) {
                printf(GREEN("Test %d passed!\n"), testNum + 1);
            } else {
                printf(RED("Test %d failed!\n"), testNum + 1);
                good = false;
            }
        }
        return good ? 0 : 1;
    }

    return 0;
}
