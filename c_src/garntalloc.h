/* file:    garntalloc.h
 * author:  garnt
 * date:    04/24/2022
 * desc:    Header for garntalloc allocator.
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void *gmalloc(size_t size);
void gfree(void *addr);