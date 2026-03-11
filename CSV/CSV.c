#include <stdio.h>
#include <string.h>

int findMaxLenWord(char array[100][100], int currentSize)
{
    int maxLen = 0;
    for (int i = 0; i < currentSize; i++) {
        int currentLen = 0;
        while (array[i][currentLen] != '\0') {
            currentLen++;
        }
        if (currentLen > maxLen) {
            maxLen = currentLen;
        }
    }
    return maxLen;
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

void printLine(FILE* out, int w1, int w2, char symbol)
{
    fprintf(out, "+");
    for (int i = 0; i < w1 + 2; i++) {
        fprintf(out, "%c", symbol);
    }
    fprintf(out, "+");
    for (int i = 0; i < w2 + 2; i++) {
        fprintf(out, "%c", symbol);
    }
    fprintf(out, "+\n");
}

int main()
{
    FILE* fileIn = fopen("input.csv", "r");
    FILE* fileOut = fopen("output.txt", "w");

    if (!fileIn || !fileOut) {
        return 1;
    }

    char buffer[100];
    char right[100][100];
    char left[100][100];
    int size = 0;

    while (fgets(buffer, 100, fileIn) != NULL) {
        int counter = 0;
        int counterForWhile = 0;
        int flag = 0;
        while (buffer[counterForWhile] != '\n' && buffer[counterForWhile] != '\r' && buffer[counterForWhile] != '\0') {
            if (flag == 0 && buffer[counterForWhile] != ',') {
                left[size][counter] = buffer[counterForWhile];
                counter++;
            } else if (buffer[counterForWhile] == ',') {
                left[size][counter] = '\0';
                flag = 1;
                counter = 0;
            } else {
                right[size][counter] = buffer[counterForWhile];
                counter++;
            }
            counterForWhile++;
        }
        right[size][counter] = '\0';
        size++;
    }

    int maxL = findMaxLenWord(left, size);
    int maxR = findMaxLenWord(right, size);

    printLine(fileOut, maxL, maxR, '=');

    for (int i = 0; i < size; i++) {
        fprintf(fileOut, "| %-*s | ", maxL, left[i]);

        if (i > 0 && isNumber(right[i])) {
            fprintf(fileOut, "%*s |\n", maxR, right[i]);
        } else {
            fprintf(fileOut, "%-*s |\n", maxR, right[i]);
        }

        if (i == 0) {
            printLine(fileOut, maxL, maxR, '=');
        } else {
            printLine(fileOut, maxL, maxR, '-');
        }
    }

    fclose(fileIn);
    fclose(fileOut);
    return 0;
}
