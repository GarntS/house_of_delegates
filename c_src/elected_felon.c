/* file:    elected_felon.c
 * author:  garnt
 * date:    04/26/2022
 * desc:    A DawgCTF menu heap challenge.
 */

#include <stdio.h>
#include "garntalloc.h"

#define INPUTS_MAX 5

// do_menu() does the menu
void do_menu()
{
    char c;
    // input loop
    for (int inputs = 0; inputs <= INPUTS_MAX; inputs++) {
        // print menu and prompt
        printf("1 - Alloc A\n");
        printf("2 - Alloc B\n");
        printf("3 - Free\n");
        printf("4 - Call\n");
        printf("5 - Exit\n");
        printf("Pick an option: ");

        // get input
        c = getchar();

        // continue if input not in range
        if (c < '1' || c > '5') {
            inputs--;
            continue;
        }
        
        switch (c) {
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                printf("Exiting...\n");
                exit(0);
                break;
        }
    }
}

// main() is the entrypoint
int main()
{
    do_menu();
}