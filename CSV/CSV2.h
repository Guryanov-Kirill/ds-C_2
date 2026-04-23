#pragma once

#include <stdio.h>

typedef struct {
    char*** matrix; // содержимое ячеек
    int* colMaxElement; // максимальная длина ячейки в каждом столбце
    int rowCount; // сколько строк заполнено
    int colCount; // сколько столбцов заполнено
    int rowCapacity; // сколько строк выделено в памяти
    int colCapacity; // сколько столбцов выделено в памяти
} CSVTable;

// освобождает всю память таблицы
void freeTable(CSVTable* t);

// выделяет начальную память, возвращает 1 если успешно, 0 если нет
int initTable(CSVTable* t);

// если нет места для новой строки, удваивает количество строк, возвращает 0 при ошибке
int ensureRowCapacity(CSVTable* t);

// если нет места для столбца, удваивает количество столбцов, возвращает 0 при ошибке
int ensureColCapacity(CSVTable* t, int col);

// заполняет colMaxElement: для каждого столбца находит длину самой длинной ячейки
void findMaxElement(CSVTable* t);

// возвращает 1 если строка является числом, иначе 0
int isNumber(char* s);

// печатает разделительную линию
void printLine(FILE* out, CSVTable* t, char symbol);

// разбирает одну строку CSV и добавляет её в таблицу, возвращает 0 при ошибке памяти
int parseRow(CSVTable* t, char* buffer);

// печатает строку таблицы
void printRow(FILE* fileOut, CSVTable* t, int i);
