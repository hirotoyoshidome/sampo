#include <assert.h>
#include <stdio.h>

#include "test_controller.c"
#include "test_poi.c"

typedef void (*TestFunc)(void);

typedef struct {
    const char *name;
    TestFunc func;
} TestCase;

int main() {
    printf("----------- Start Test Case -----------\n");

    // NOTE: Test Case Defined here.
    TestCase tests[] = {{"test_controller_001", test_controller_001},
                        {"test_poi_001", test_poi_001}};

    int total_testcase = sizeof(tests) / sizeof(TestCase);
    for (int i = 0; i < total_testcase; i++) {
        printf("Run: %s\n", tests[i].name);
        tests[i].func();
        printf("Pass: %s\n", tests[i].name);
    }

    printf("----------- Finish Test Case!!! -----------\n");
    printf("Total TestCase : %d\n", total_testcase);

    return 0;
}
