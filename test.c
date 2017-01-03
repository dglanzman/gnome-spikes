#include <glib.h>

void test_1(void) {
    g_assert_cmpint(1, ==, 1);
}

int main(int argc, char * argv[]) {
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/all/test_1", test_1);
    return g_test_run();
}
