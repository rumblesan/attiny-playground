#include <stdio.h>

#include "minunit.h"

#define test_header(text) \
    printf("\n\n"); \
    printf("***********\n"); \
    printf(text);

MU_TEST(starting_test) {
    mu_assert_int_eq(0, 0);
}

MU_TEST_SUITE(starting_test_suite) {
    test_header("Starting tests\n");
    MU_RUN_TEST(starting_test);
}

int main(int argc, char *argv[]) {
    test_header("ATTiny Playground Tests\n");
    MU_RUN_SUITE(starting_test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
