#include "nctest.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

extern NT_Test __start_test_registry;
extern NT_Test __stop_test_registry;

NT_TestState _nctest_curr_test_state = {
    .test_name = NULL,
    .fail_file = NULL,
    .fail_line = 0,
    .failed = false // will be set to false by asserts in case they fail
};

static struct {
    size_t passed_count;
    size_t failed_count;
} g_test_stat = {0, 0};

void nctest_run_all()
{
    for (NT_Test* t = &__start_test_registry; t < &__stop_test_registry; t++)
    {
        _nctest_curr_test_state = (NT_TestState) {.test_name = t->name };

        if (t->fn == NULL)
            continue;

        t->fn();

        if (_nctest_curr_test_state.failed) {
            ++g_test_stat.failed_count;
        } else {
            ++g_test_stat.passed_count;
        }
    }

    printf("TEST RESULTS\n");
    printf("Tests run: %zu\n", g_test_stat.passed_count + g_test_stat.failed_count);
    printf("Tests passed: %zu\n", g_test_stat.passed_count);
    printf("Tests failed: %zu\n\n", g_test_stat.failed_count);
}
