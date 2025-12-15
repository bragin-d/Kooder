#include <stdio.h>

void my_custom_function() {
    printf("Hello from my_custom_function!\n");
}
// START
void test_func()
{
    printf("Another test func\n");
}

void dummy_func()
{
    printf(" ");
}
// END

int main() {
    printf("Hello from main!\n");
    my_custom_function();
    return 0;
}
