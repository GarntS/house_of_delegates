/* file:    test_garntalloc.c
 * author:  garnt
 * date:    04/24/2022
 * desc:    Test for garntalloc allocator.
 */

#include <stdio.h>
#include "garntalloc.h"

// main() is the entrypoint
int main()
{
    int *i = NULL;
    i = (int*)gmalloc(sizeof(int));
    *i = 69;
    printf("Should be 69: %d | %p\n", *i, i);
    gfree(i);

    for (int i = 0; i < 100; i++) {
        printf("%p\n", gmalloc(sizeof(int)));
    }

    return 0;
}