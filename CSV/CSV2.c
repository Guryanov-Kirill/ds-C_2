#include <stdio.h>
#include <string.h>

typedef struct {
    char matrix[300][300][100];
    int colMaxElement[300];
    int rowCount;
    int colCount;
} CSVTable;

void findMaxElement(CSVTable* t)
{
    for (int j = 0; j <= t->colCount; j++) {
        int maxLen = 0;
        for (int i = 0; i < t->rowCount; i++) {
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

int main()
{
    FILE* fileIn = fopen("input.csv", "r");
    FILE* fileOut = fopen("output2.txt", "w");

    if (!fileIn || !fileOut) {
        return 1;
    }

    static CSVTable t = { 0 };
    char buffer[300];
    while (fgets(buffer, 300, fileIn) != NULL) {
        int i = 0;
        int column = 0;
        int number = 0;
        while ((buffer[i] != '\n') && (buffer[i] != '\r') && (buffer[i] != '\0')) {
            if (buffer[i] != ',') {
                t.matrix[t.rowCount][column][number] = buffer[i];
                number++;
            } else {
                t.matrix[t.rowCount][column][number] = '\0';
                column++;
                number = 0;
            }
            i++;
        }
        t.matrix[t.rowCount][column][number] = '\0';
        if (column > t.colCount) {
            t.colCount = column;
        }
        t.rowCount++;
    }

    findMaxElement(&t);

    for (int i = 0; i < t.rowCount; i++) {
        if (i <= 1) {
            printLine(fileOut, &t, '=');
        } else {
            printLine(fileOut, &t, '-');
        }
        for (int j = 0; j <= t.colCount; j++) {
            if (i == 0) {
                fprintf(fileOut, "| %-*s ", t.colMaxElement[j], t.matrix[i][j]);
            } else {
                if (isNumber(t.matrix[i][j])) {
                    fprintf(fileOut, "| %*s ", t.colMaxElement[j], t.matrix[i][j]);
                } else {
                    fprintf(fileOut, "| %-*s ", t.colMaxElement[j], t.matrix[i][j]);
                }
            }
        }
        fprintf(fileOut, "|");
        fprintf(fileOut, "\n");
    }
    printLine(fileOut, &t, '-');
    fclose(fileIn);
    fclose(fileOut);
    return 0;
}