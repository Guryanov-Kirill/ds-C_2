#include <stdio.h>
#include <string.h>

void findMaxElement(int maxArray[300], char array[300][300][100], int rows, int cols)
{
    for (int j = 0; j <= cols; j++) {
        int maxLen = 0;
        for (int i = 0; i < rows; i++) {
            int current = 0;
            while (array[i][j][current] != '\0') {
                current++;
            }
            if (current > maxLen) {
                maxLen = current;
            }
            maxArray[j] = maxLen;
        }
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

void printLine(FILE* out, int maxElement[300], int cols, char symbol)
{
    for (int j = 0; j <= cols; j++) {
        fprintf(out, "+");
        for (int k = 0; k < maxElement[j] + 2; k++) {
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

    char buffer[300];
    static char matrix[300][300][100];
    int row = 0;
    int maxColumn = 0;
    while (fgets(buffer, 300, fileIn) != NULL) {
        int i = 0;
        int column = 0;
        int number = 0;
        while ((buffer[i] != '\n') && (buffer[i] != '\r')) {
            if (buffer[i] != ',') {
                matrix[row][column][number] = buffer[i];
                number++;
            } else {
                matrix[row][column][number] = '\0';
                column++;
                number = 0;
            }
            i++;
        }
        matrix[row][column][number] = '\0';
        if (column > maxColumn) {
            maxColumn = column;
        }
        row++;
    }

    int maxElement[300];
    findMaxElement(maxElement, matrix, row, maxColumn);

    for (int i = 0; i < row; i++) {
        if (i <= 1) {
            printLine(fileOut, maxElement, maxColumn, '=');
        } else {
            printLine(fileOut, maxElement, maxColumn, '-');
        }
        for (int j = 0; j <= maxColumn; j++) {
            if (i == 0) {
                fprintf(fileOut, "| %-*s ", maxElement[j], matrix[i][j]);
            } else {
                if (isNumber(matrix[i][j])) {
                    fprintf(fileOut, "| %*s ", maxElement[j], matrix[i][j]);
                } else {
                    fprintf(fileOut, "| %-*s ", maxElement[j], matrix[i][j]);
                }
            }
        }
        fprintf(fileOut, "|");
        fprintf(fileOut, "\n");
    }
    printLine(fileOut, maxElement, maxColumn, '-');
    fclose(fileIn);
    fclose(fileOut);
    return 0;
}