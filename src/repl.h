#include <Arduino.h>
#include "betterprinting.hpp"

#define USE_SERIAL      1
#define USE_TELNET      2
#define USE_BOTH        3

#define PRINT_METHOD    USE_SERIAL
#define INPUT_METHOD    USE_SERIAL

#define PASSWORD        "ebuilds"

void print_prompt();
void repl_loop();

typedef enum _policy {
    BASICUSER,
    SUPERUSER
} policy_t;

typedef struct _commands {
    const char * name;
    void (*f_ptr)();
    policy_t pol;
    const char * desc;
} commands_t;