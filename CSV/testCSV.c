#include "CSV2.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

bool testTableInit(void)
{
    CSVTable t;
    if (initTable(&t) == 0)
        return false;
    bool res = (t.rowCount == 0 && t.matrix != NULL);
    freeTable(&t);
    return res;
}

bool testParseRow(void)
{
    CSVTable t;
    initTable(&t);
    char buffer[] = "A,B,C\n";
    if (parseRow(&t, buffer) == 0) {
        freeTable(&t);
        return false;
    }
    bool res = (t.rowCount == 1 && t.colCount == 2);
    if (res) {
        res = (strcmp(t.matrix[0][0], "A") == 0 && strcmp(t.matrix[0][1], "B") == 0);
    }
    freeTable(&t);
    return res;
}

bool testRowExpansion(void)
{
    CSVTable t;
    initTable(&t);
    t.rowCapacity = 1;
    parseRow(&t, "1,1\n");
    if (parseRow(&t, "2,2\n") == 0) {
        freeTable(&t);
        return false;
    }
    bool res = (t.rowCount == 2 && t.rowCapacity >= 2);
    freeTable(&t);
    return res;
}

bool testColExpansion(void)
{
    CSVTable t;
    initTable(&t);
    t.colCapacity = 1;
    if (parseRow(&t, "A,B,C,D\n") == 0) {
        freeTable(&t);
        return false;
    }
    bool res = (t.colCount == 3 && t.colCapacity >= 4);
    freeTable(&t);
    return res;
}

bool testMaxElement(void)
{
    CSVTable t;
    initTable(&t);
    parseRow(&t, "Word,LongerWord\n");
    findMaxElement(&t);
    bool res = (t.colMaxElement[0] == 4 && t.colMaxElement[1] == 10);
    freeTable(&t);
    return res;
}

bool testIsNumber(void)
{
    return (isNumber("123") == 1 && isNumber("abc") == 0 && isNumber("45.6") == 1);
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
        const int TEST_NUM = 6;
        bool (*tests[])(void) = {
            &testTableInit,
            &testParseRow,
            &testRowExpansion,
            &testColExpansion,
            &testMaxElement,
            &testIsNumber
        };

        bool ok = true;
        for (int i = 0; i < TEST_NUM; ++i) {
            if (tests[i]()) {
                printf(GREEN("Test %d passed!\n"), i + 1);
            } else {
                printf(RED("Test %d failed!\n"), i + 1);
                ok = false;
            }
        }
        return ok ? 0 : 1;
    }
    return 0;
}
