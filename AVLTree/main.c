#include "AVLTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{
    FILE* file = fopen("airports.txt", "r");
    char line[310];
    AVL* avl = newAvl();
    while (fgets(line, sizeof(line), file)) {
        char key[10];
        char name[300];
        int i = 0;
        while (line[i] != ':') {
            key[i] = line[i];
            i++;
        }
        key[i] = '\0';
        int j = 0;
        i++;
        while (line[i] != '\n') {
            name[j] = line[i];
            i++;
            j++;
        }
        name[j] = '\0';
        avl->root = insert(avl->root, key, name);
        avl->size++;
    }
    fclose(file);
    printf("Загружено %d аэропортов. Система готова к работе.\n", avl->size);

    char command[20];
    char argument[350];

    while (1) {
        printf("\n> ");
        scanf("%s", command);
        if (strcmp(command, "find") == 0) {
            scanf("%s", argument);
            Node* result = find(avl->root, argument);
            if (result != NULL) {
                printf("%s - %s\n", result->key, result->name);
            } else {
                printf("Аэропорт с кодом '%s' не найден в базе.\n", argument);
            }
        } else if (strcmp(command, "insert") == 0) {
            fgets(argument, sizeof(argument), stdin);
            int i = 1;
            char key[10];
            char name[300];
            while (argument[i] != ':') {
                key[i - 1] = argument[i];
                i++;
            }
            key[i - 1] = '\0';
            i++;
            int j = 0;
            while (argument[i] != '\n') {
                name[j] = argument[i];
                i++;
                j++;
            }
            name[j] = '\0';
            avl->root = insert(avl->root, key, name);
            avl->size++;
            printf("Аэропорт '%s' добавлен в базу.\n", key);
        } else if (strcmp(command, "delete") == 0) {
            scanf("%s", argument);
            avl->root = deleteNode(avl->root, argument);
            avl->size--;
            printf("Аэропорт '%s' удалён из базы.\n", argument);
        } else if (strcmp(command, "save") == 0) {
            FILE* file = fopen("airports.txt", "w");
            saveFile(avl->root, file);
            printf("База сохранена: %d аэропортов.\n", avl->size);
            fclose(file);
        } else if (strcmp(command, "quit") == 0) {
            break;
        }
    }
    deleteAvl(avl);
    return 0;
}