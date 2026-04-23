#include "CSV2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeTable(CSVTable* t)
{
    for (int i = 0; i < t->rowCapacity; i++) {
        if (t->matrix[i]) {
            for (int j = 0; j < t->colCapacity; j++) {
                free(t->matrix[i][j]);
            }
            free(t->matrix[i]);
        }
    }
    free(t->matrix);
    free(t->colMaxElement);
}

int initTable(CSVTable* t)
{
    t->rowCapacity = 32;
    t->colCapacity = 32;
    t->rowCount = 0;
    t->colCount = 0;

    t->matrix = malloc(t->rowCapacity * sizeof(*(t->matrix)));
    if (!t->matrix)
        return 0;

    t->colMaxElement = malloc(t->colCapacity * sizeof(int));
    if (!t->colMaxElement) {
        free(t->matrix);
        t->matrix = NULL;
        return 0;
    }

    for (int i = 0; i < t->rowCapacity; i++) {
        t->matrix[i] = calloc(t->colCapacity, sizeof(char*));
        if (!t->matrix[i]) {
            for (int k = i; k < t->rowCapacity; k++)
                t->matrix[k] = NULL;
            freeTable(t);
            return 0;
        }
    }
    return 1;
}

int ensureRowCapacity(CSVTable* t)
{
    if (t->rowCount < t->rowCapacity)
        return 1;

    int newCap = t->rowCapacity * 2;
    char*** newMatrix = realloc(t->matrix, newCap * sizeof(char**));
    if (!newMatrix)
        return 0;
    t->matrix = newMatrix;

    for (int i = t->rowCapacity; i < newCap; i++) {
        t->matrix[i] = calloc(t->colCapacity, sizeof(char*));
        if (!t->matrix[i]) {
            for (int k = i; k < newCap; k++)
                t->matrix[k] = NULL;
            t->rowCapacity = newCap;
            return 0;
        }
    }
    t->rowCapacity = newCap;
    return 1;
}

int ensureColCapacity(CSVTable* t, int col)
{
    if (col < t->colCapacity)
        return 1;

    int newCap = t->colCapacity * 2;
    while (col >= newCap)
        newCap *= 2;

    int* newColMax = realloc(t->colMaxElement, newCap * sizeof(int));
    if (!newColMax)
        return 0;
    t->colMaxElement = newColMax;

    for (int i = 0; i < t->rowCapacity; i++) {
        char** newRow = realloc(t->matrix[i], newCap * sizeof(char*));
        if (!newRow) {
            return 0;
        }
        t->matrix[i] = newRow;
        for (int j = t->colCapacity; j < newCap; j++) {
            t->matrix[i][j] = NULL;
        }
    }
    t->colCapacity = newCap;
    return 1;
}

void findMaxElement(CSVTable* t)
{
    for (int j = 0; j <= t->colCount; j++) {
        int maxLen = 0;
        for (int i = 0; i < t->rowCount; i++) {
            if (t->matrix[i][j] == NULL)
                continue;
            int current = strlen(t->matrix[i][j]);
            if (current > maxLen) {
                maxLen = current;
            }
        }
        t->colMaxElement[j] = maxLen;
    }
}

int isNumber(char* s)
{
    if (*s == '\0') {
        return 0;
    }
    for (int i = 0; s[i] != '\0'; i++) {
        if ((s[i] < '0' || s[i] > '9') && s[i] != '.' && s[i] != '-') {
            return 0;
        }
    }
    return 1;
}

void printLine(FILE* out, CSVTable* t, char symbol)
{
    for (int j = 0; j <= t->colCount; j++) {
        fprintf(out, "+");
        for (int k = 0; k < t->colMaxElement[j] + 2; k++) {
            fprintf(out, "%c", symbol);
        }
    }
    fprintf(out, "+\n");
}

int parseRow(CSVTable* t, char* buffer)
{
    if (!ensureRowCapacity(t))
        return 0;

    int i = 0;
    int column = 0;
    int start = 0;

    while (1) {
        if (buffer[i] == ',' || buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == '\0') {
            if (!ensureColCapacity(t, column))
                return 0;

            int len = i - start;
            free(t->matrix[t->rowCount][column]);

            t->matrix[t->rowCount][column] = calloc(len + 1, sizeof(char));
            if (!t->matrix[t->rowCount][column])
                return 0;

            strncpy(t->matrix[t->rowCount][column], buffer + start, len);
            t->matrix[t->rowCount][column][len] = '\0';

            if (column > t->colCount)
                t->colCount = column;

            if (buffer[i] == ',') {
                column++;
                start = i + 1;
            } else {
                break;
            }
        }
        i++;
    }
    t->rowCount++;
    return 1;
}

void printRow(FILE* fileOut, CSVTable* t, int i)
{
    for (int j = 0; j <= t->colCount; j++) {
        char* cell;

        if (t->matrix[i][j] != NULL) {
            cell = t->matrix[i][j];
        } else {
            cell = "";
        }

        if (i == 0) {
            fprintf(fileOut, "| %-*s ", t->colMaxElement[j], cell);
        } else {
            if (isNumber(cell) == 1) {
                fprintf(fileOut, "| %*s ", t->colMaxElement[j], cell);
            } else {
                fprintf(fileOut, "| %-*s ", t->colMaxElement[j], cell);
            }
        }
    }
    fprintf(fileOut, "|\n");
}
