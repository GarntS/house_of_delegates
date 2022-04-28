/* file:    house_of_delegates.c
 * author:  garnt
 * date:    04/26/2022
 * desc:    Another DawgCTF menu heap challenge.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "garntalloc.h"

#define INPUTS_MAX 5
#define MAX_BILL_LEN 60
#define MAX_DELEGATE_NAME_LEN 20

// enum the different possible allocation types
typedef enum _alloc_type {
    BILL_TYPE,
    DELEGATE_TYPE
} alloc_type;

// array to handle converting from enum to str
char *alloc_type_str[2] = {
    "Bill",
    "Delegate"
};

// array to handle the allocations made
typedef struct _app_allocation {
    unsigned char is_active;
    alloc_type type;
    void *addr;
    size_t size;
} app_allocation;
app_allocation allocations[INPUTS_MAX];

// struct representing a delegate
typedef struct _delegate {
    void (*call_to_stand)(struct _delegate*);
    char name[MAX_DELEGATE_NAME_LEN + 1];
} delegate;

// announce_name() gives the delegate something to do when called
void announce_name(delegate *person)
{
    printf("Hello, my name is Delegate %s\n", person->name);
}

// print_allocations() prints out the active members in the allocations struct
void print_allocations()
{
    printf("Allocations: \n");
    for (size_t i = 0; i < INPUTS_MAX; i++) {
        if (!allocations[i].is_active)
            continue;
        printf("[%lu]: %s\n", i, alloc_type_str[allocations[i].type]);
    }
}

// do_menu() does the menu
void do_menu()
{
    char c;
    // input loop
    for (int inputs = 0; inputs <= INPUTS_MAX; inputs++) {
        // print menu and prompt
        printf("Allocations remaining: %d\n", INPUTS_MAX - inputs);
        printf("1 - Alloc Bill\n");
        printf("2 - Alloc Delegate\n");
        printf("3 - Free\n");
        printf("4 - Call Delegate to Speak\n");
        printf("5 - Exit\n");
        printf("Pick an option: ");

        // get input, eat the '\n'
        c = getchar();
        getchar();

        // continue if input not in range
        if (c < '1' || c > '5') {
            inputs--;
            continue;
        }
        
        // some variables for this switch statement
        char submenu_option = 0;
        app_allocation *cur_allocation = NULL;
        char input_buf[MAX_BILL_LEN];
        size_t input_len = MAX_BILL_LEN;
        delegate *the_delegate = NULL;
        switch (c) {
            // alloc bill
            case '1':
                // get the new bill's text
                printf("Enter bill text: ");
                fgets(input_buf, MAX_BILL_LEN, stdin);
                input_len = strnlen(input_buf, MAX_BILL_LEN);

                // update allocations struct
                cur_allocation = allocations + inputs;
                cur_allocation->size = input_len - sizeof(uintptr_t);
                cur_allocation->is_active = 1;
                cur_allocation->type = BILL_TYPE;
                cur_allocation->addr = gmalloc(cur_allocation->size);
                
                // memcpy() the name over
                memcpy(cur_allocation->addr, input_buf, input_len);
                printf("Allocated new bill.\n");
                break;

            // alloc delegate
            case '2':
                // update allocations struct
                cur_allocation = allocations + inputs;
                cur_allocation->size = sizeof(delegate);
                cur_allocation->is_active = 1;
                cur_allocation->type = DELEGATE_TYPE;
                cur_allocation->addr = gmalloc(sizeof(delegate));

                // construct the new delegate and tell them to announce their
                // name when called to the stand
                the_delegate = (delegate*)cur_allocation->addr;
                the_delegate->call_to_stand = announce_name;

                printf("Enter the new delegate's name: ");
                fgets(the_delegate->name, MAX_DELEGATE_NAME_LEN, stdin);
                break;

            // free
            case '3':
                print_allocations();
                printf("Choose an allocation to free. Index: ");
                submenu_option = getchar();

                // turn the option into an index
                if (submenu_option < '0')
                    continue;
                submenu_option -= '0';

                // bound the options to the allocations array size
                if (submenu_option < 0 || submenu_option >= INPUTS_MAX)
                    continue;
                
                // make sure the allocation exists
                if (!allocations[submenu_option].is_active)
                    continue;
                
                // free it, update allocations struct
                gfree(allocations[submenu_option].addr);
                allocations[submenu_option].is_active = 0;
                break;

            // call
            case '4':
                print_allocations();
                printf("Choose a delegate to call. Index: ");
                submenu_option = getchar();

                // turn the option into an index
                if (submenu_option < '0')
                    continue;
                submenu_option -= '0';

                // bound the options to the allocations array size
                if (submenu_option < 0 || submenu_option >= INPUTS_MAX)
                    continue;
                
                // make sure the allocation exists
                if (!allocations[submenu_option].is_active)
                    continue;

                // make sure the allocation is a delegate
                if (allocations[submenu_option].type != DELEGATE_TYPE)
                    continue;
                
                // cast to a delegate and call it to the stand
                the_delegate = (delegate*)allocations[submenu_option].addr;
                the_delegate->call_to_stand(the_delegate);
                break;

            // exit
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
    // go ahead and leak libc because i'm a benevolent god.
    printf("printf @ %p. Have fun.\n", printf);

    do_menu();
}