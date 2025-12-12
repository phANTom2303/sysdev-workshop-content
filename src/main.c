#include <stdio.h>
#include "utils.h"

int main() {
    printf("Main program started.\n");
    print_hello();
    
    int sum = add(5, 3);
    printf("5 + 3 = %d\n", sum);
    
    return 0;
}
