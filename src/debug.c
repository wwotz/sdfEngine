#include "../include/sdfE.h"

static char *sdfe_debug_stack[SDFE_DEBUG_STACK_CAPACITY] = {0};
static size_t sdfe_debug_stack_size = 0;
static size_t sdfe_debug_stack_ptr = 0;

static int sdfe_debug_stack_full()
{
        return sdfe_debug_stack_size == SDFE_DEBUG_STACK_CAPACITY;
}

static int sdfe_debug_stack_empty()
{
        return sdfe_debug_stack_size == 0;
}

void sdfe_debug_stack_push(const char *msg, const char *file, size_t line)
{
        if (!sdfe_debug_stack_full())
                sdfe_debug_stack_size++;

        if (sdfe_debug_stack[sdfe_debug_stack_ptr])
                free(sdfe_debug_stack[sdfe_debug_stack_ptr]);

        sdfe_debug_stack[sdfe_debug_stack_ptr] = malloc(SDFE_DEBUG_MESSAGE_LENGTH);
        if (!sdfe_debug_stack[sdfe_debug_stack_ptr]) {
                fprintf(stderr, "Fatal: Ran out of memory for debugging! Exiting...\n");
                exit(EXIT_FAILURE);
        }

        snprintf(sdfe_debug_stack[sdfe_debug_stack_ptr], SDFE_DEBUG_MESSAGE_LENGTH,
                 "%s@%ld: %s", file, line, msg);
        sdfe_debug_stack_ptr = (sdfe_debug_stack_ptr + 1)
                % SDFE_DEBUG_STACK_CAPACITY;
}

const char *sdfe_debug_stack_pop(void)
{
        if (sdfe_debug_stack_empty())
                return NULL;
        sdfe_debug_stack_size--;
        sdfe_debug_stack_ptr--;

        if (sdfe_debug_stack_ptr < 0)
                sdfe_debug_stack_ptr += SDFE_DEBUG_STACK_CAPACITY;
        return sdfe_debug_stack[sdfe_debug_stack_ptr];
}

int sdfe_debug_had_error(void)
{
        return !sdfe_debug_stack_empty();
}
