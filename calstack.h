/* calstack.h
 * Created by Wang Haoren
 * --------------------------------
 * Declaration of the fundamental data structures and their operations
 */


#ifndef CALSTACK_CALSTACK_H_
#define CALSTACK_CALSTACK_H_

#include <stddef.h>
#include "numeric.h"
#include "operation.h"

#define CALSTACK_RESIZE_FACTOR 2
#define CALSTACK_INIT_SIZE 16

//Stack: Store numbers

typedef struct {
    Numeric *head;
    Numeric *tail;
    size_t capacity;
} Stack;

void Stack_new(Stack *stack);                               // Create a new stack
void Stack_free(Stack *stack);                              // Free an existed stack
void Stack_push(Stack *stack, const Numeric *value);        // Push an element into the stack
void Stack_pop(Stack *stack,
               Numeric *address);                           // Pop an element out of the stack, address stores the popped value
size_t Stack_size(const Stack *stack);                      // Returns the size of a stack

//SStack: Store symbols

typedef struct {
    Operation *head;
    Operation *tail;
    size_t capacity;
} SStack;

void SStack_new(SStack *stack);                               // Create a new stack
void SStack_free(SStack *stack);                              // Free an existed stack
void SStack_push(SStack *stack, Operation value);             // Push an element into the stack
Operation
SStack_pop(SStack *stack);                          // Pop an element out of the stack, address stores the popped value
size_t SStack_size(const SStack *stack);                      // Returns the size of a stack
Operation SStack_top(const SStack *stack);

bool SStack_empty(const SStack *stack);                      // Returns true if the stack is empty

#endif //CALSTACK_CALSTACK_H_
