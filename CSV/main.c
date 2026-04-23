#include "CSV2.h"
#include <stdio.h>

int main(void)
{
    FILE* fileIn = fopen("input.csv", "r");
    FILE* fileOut = fopen("output.txt", "w");

    if (fileIn == NULL || fileOut == NULL) {
        if (fileIn != NULL)
            fclose(fileIn);
        if (fileOut != NULL)
            fclose(fileOut);
        return 1;
    }

    CSVTable t;
    if (initTable(&t) == 0) {
        fclose(fileIn);
        fclose(fileOut);
        return 1;
    }

    char buffer[5000];
    while (fgets(buffer, 5000, fileIn) != NULL) {
        if (parseRow(&t, buffer) == 0) {
            fclose(fileIn);
            fclose(fileOut);
            freeTable(&t);
            return 1;
        }
    }

    findMaxElement(&t);

    int i;
    for (i = 0; i < t.rowCount; i++) {
        if (i <= 1) {
            printLine(fileOut, &t, '=');
        } else {
            printLine(fileOut, &t, '-');
        }
        printRow(fileOut, &t, i);
    }

    printLine(fileOut, &t, '-');
    fclose(fileIn);
    fclose(fileOut);
    freeTable(&t);
    return 0;
}
