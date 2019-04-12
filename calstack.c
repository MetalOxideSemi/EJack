/* calstack.c
 * Created by Wang Haoren
 * --------------------------------
 * Impletentation of the fundamental data structures
 */
#include "calstack.h"
#include <stdlib.h>
#include <string.h>

void Stack_new(Stack *stack) {
    stack->head = malloc(CALSTACK_INIT_SIZE * sizeof(Numeric));
    stack->tail = stack->head;
    stack->capacity = CALSTACK_INIT_SIZE;
}

void Stack_free(Stack *stack) {
    free(stack->head);
    stack->head = stack->tail = NULL;
    stack->capacity = 0;
}

void Stack_push(Stack *stack, const Numeric *value) {
    if (stack->tail - stack->head == stack->capacity) {
        Numeric *temp = malloc(CALSTACK_RESIZE_FACTOR * stack->capacity * sizeof(Numeric));
        memcpy(temp, stack->head, stack->capacity * sizeof(Numeric));
        free(stack->head);
        stack->head = temp;
        stack->tail = stack->head + stack->capacity;
        stack->capacity *= CALSTACK_RESIZE_FACTOR;
    }
    Numeric* top=stack->tail++;
    Numeric_new(top);
    Numeric_assign(top, value);
}

void Stack_pop(Stack *stack, Numeric *address) {
    Numeric_assign(address, --stack->tail);
    Numeric_free(stack->tail);
}

size_t Stack_size(const Stack *stack) {
    return stack->tail - stack->head;
}

void SStack_new(SStack *stack) {
    stack->head = malloc(CALSTACK_INIT_SIZE * sizeof(Operation));
    stack->tail = stack->head;
    stack->capacity = CALSTACK_INIT_SIZE;
}

void SStack_free(SStack *stack) {
    free(stack->head);
    stack->head = stack->tail = NULL;
    stack->capacity = 0;
}

void SStack_push(SStack *stack, Operation value) {
    if (stack->tail - stack->head == stack->capacity) {
        Operation *temp = malloc(CALSTACK_RESIZE_FACTOR * stack->capacity * sizeof(Operation));
        memcpy(temp, stack->head, stack->capacity * sizeof(Operation));
        free(stack->head);
        stack->head = temp;
        stack->tail = stack->head + stack->capacity;
        stack->capacity *= CALSTACK_RESIZE_FACTOR;
    }
    *stack->tail++ = value;
}

Operation SStack_pop(SStack *stack) {
    return *--stack->tail;
}

size_t SStack_size(const SStack *stack) {
    return stack->tail - stack->head;
}

Operation SStack_top(const SStack *stack) {
    return *(stack->tail-1);
}

bool SStack_empty(const SStack *stack) {
    return stack->head == stack->tail;
}