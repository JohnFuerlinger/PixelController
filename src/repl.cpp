#include "repl.h"

#define BUFFERLEN       35

#if PRINT_METHOD == USE_SERIAL
#define HCHAROUT        CHAROUTS
#define HPUTS           SPUTS
#define HPRINTF         JPRINTF
#elif PRINT_METHOD == USE_TELNET
#define HCHAROUT        CHAROUTT
#define HPUTS           TPUTS
#define HPRINTF         TPRINTF
#elif PRINT_METHOD == USE_BOTH
#define HCHAROUT        CHAROUTB
#define HPUTS           BPUTS
#define HPRINTF         BPRINTF
#else
#error "Print method not supported. Please set 'PRINT_METHOD' to a valid value"
#endif

#if INPUT_METHOD == USE_SERIAL
#define H_AVAILABLE     Serial.available
#define H_READ          Serial.read
#define H_PEEK          Serial.peek
#elif INPUT_METHOD == USE_TELNET
#define H_AVAILABLE     TelnetStream.available
#define H_READ          TelnetStream.read
#define H_PEEK          TelnetStream.peek
#else
#error "Input method not supported. Please set 'PRINT_METHOD' to a valid value"
#endif

static void do_help();
static void do_rst();
static void do_auth();

static commands_t commands[] = {
    {"help",        do_help,            BASICUSER,      "Show the help"},
    {"rst",         do_rst,             SUPERUSER,      "Restart the ESP32"},
    {"auth",        do_auth,            BASICUSER,      "authenticate"},
};
static uint16_t numcommands = sizeof(commands) / sizeof(commands[0]);

/* Globals */
static policy_t useraccess = BASICUSER;
extern char mySSID[20];
extern char myPASSWORD[20];

void print_prompt() {
    if (useraccess == SUPERUSER) {
        HPUTS("(admin) ESP32> ");
    } else {
        HPUTS("ESP32> ");
    }
}

static void resetbuf(char * buf) {
    for (uint8_t i = 0; i < BUFFERLEN; i++) {
        buf[i] = '\0';
    }
}

/* Configure this for diffent allowable number and size input args */
#define NUMARGS         4
#define ARGLEN          15
static size_t numargs = 0;
static char args[NUMARGS][ARGLEN];

/* Populate args into globals */
static void parse_args(char * buf) {
    /* Try to find the first space (there may not be one) */
    int i = 0;
    int tmpargcnt = 0;
    size_t buflen = strlen(buf);
    while (i < buflen && buf[i] != ' ') { i++; }
    for (int arg = 0; arg < NUMARGS; arg++) {
        i++;

        /* See if we're at the end of the buffer - if so, we're done! */
        if (i >= buflen) { return; }

        /* If not, time to parse an arg. */
        numargs++;
        tmpargcnt = 0;
        while (i < buflen && buf[i] != ' ') {
            args[arg][tmpargcnt++] = buf[i++];
        }
    }
}

static void reset_args(void) {
    for (int i = 0; i < NUMARGS; i++) {
        for (int j = 0; j < ARGLEN; j++) {
            args[i][j] = '\0';
        }
    }
    numargs = 0;
}

/* Compare the first argument (space delimited) of input arguments and a command.
 * The input arguments could contain only a single symbol or multiple */
uint8_t compfirstarg(const char * input, const char * cmd) {
    int arglen = 0;
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') { break; }
        arglen++;
    }
    if (arglen != strlen(cmd)) {
        return 0xff;
    } else {
        return strncmp(input, cmd, arglen);
    }
}

void repl_loop() {

    static char msgbuf[BUFFERLEN];
    static uint8_t msgidx = 0;

    if (H_AVAILABLE() > 0) {
        char rchar = (char)H_READ();

        /* Execute buffer contents */
        if (rchar == '\r') {

#if INPUT_METHOD == USE_TELNET
            while (!H_AVAILABLE());
            if ((H_PEEK()) == '\0') { /* Apparently telnet is throwing this bad boy in there */
                (void)H_READ(); /* Throw one away */
#endif
                if (msgidx > 0) {
                    /* Try to process the command */
                    for (int i = 0; i < numcommands; i++) {
                        if (!compfirstarg(msgbuf, commands[i].name)) {
                            if (useraccess >= commands[i].pol) {
                                parse_args(msgbuf);
                                HPUTS("\n");
                                commands[i].f_ptr();
                                msgidx = 0;
                                resetbuf(msgbuf);
                                HPUTS("\n");
                                print_prompt();
                                reset_args();
                                return;
                            } else {
                                HPUTS("\nAccess denied.\n\n");
                                msgidx = 0;
                                resetbuf(msgbuf);
                                print_prompt();
                                return;
                            }
                        }
                    }
                    HPUTS("\nCommand not recognized.\n\n");
                    print_prompt();
                    msgidx = 0;
                    resetbuf(msgbuf);
                    return;
                } else {
                    HPUTS("\n");
                    print_prompt();
                    return;
                }
#if INPUT_METHOD == USE_TELNET
            }
#endif
        }

        /* Handle backspace */
        if (rchar == '\b') {
            if (msgidx != 0) {
                HPUTS("\b \b");
                msgbuf[msgidx-1] = '\0';
                msgidx--;
                return;
            } else {
                /* Nothing to do! */
                return;
            }
        }

        /* Handle special characters (escaped only) */
        if (rchar == '\x1b') {
            if (H_AVAILABLE() > 0 && H_PEEK() == '[') {
                (void)H_PEEK(); /* Throw this one away */

                /* The following two characters will tell us what the code is */
                unsigned long wait_micros = micros() + 1000; /* Allow 1 millisecond for the user to finish sending */
                while(H_AVAILABLE() < 1) {
                    if (micros() > wait_micros) {
                        HPUTS("Timed out waiting for special character escape sequence.\n");
                        return;
                    }
                }

                /* Now we have all our characters */
                char c1 = H_READ();

                /* These are the special characters I will support */
                /* UP ARROW */
                if (c1 == 'A') {
                    HPUTS("UP!");
                }

                /* DOWN ARROW */
                if (c1 == 'B') {
                    HPUTS("DOWN!");
                }

                /* LEFT ARROW */
                if (c1 == 'D') {
                    HPUTS("LEFT!");
                }

                /* RIGHT ARROW */
                if (c1 == 'C') {
                    HPUTS("RIGHT!");
                }

            } else {
                /* The ESC must have been a fluke... carry on */
                HPUTS("Special character not recognized.\n");
                return;
            }
            return;
        }

        /* Handle del */
        if (rchar == '\x7f') {
            /* For now, not handled */
            return;
        }

        /* Continue processing characters */
        if (msgidx < BUFFERLEN-1) {
            msgbuf[msgidx] = rchar;
            msgidx++;
            HCHAROUT(rchar);
        } else {
            HPUTS("\nCharacter buffer overflowed.\n");
            print_prompt();
            msgidx = 0;
            resetbuf(msgbuf);
        }
    }
}


static void do_help() {
    if (numargs == 1) {
        /* Try to find the arg */
        for (uint8_t i = 0; i < numcommands; i++) {
            if (strlen(commands[i].name) == strlen(args[0]) && !strncmp(args[0], commands[i].name, strlen(commands[i].name))) {
                HPRINTF("%s\n", commands[i].desc);
                return;
            }
        }
    }
    /* Print all if we couldn't find the cmd or weren't prompted for one */
    HPUTS("\n- - - - - - - - - AVAILABLE COMMANDS - - - - - - - - -\n ");
    /* Print the commands in order, in 3 columns */
    for (int i = 0; i < numcommands; i++) {
        HPRINTF("%s\t\t", commands[i].name);
        if ((i+1) % 4 == 0) { HPUTS("\n "); }
    }
    HPUTS("\n");
}

static void do_rst() {
    ESP.restart();
}

static void do_auth() {
    if (numargs == 1) {
        if (!strncmp(args[0], "-", 1)) {
            /* De-authenticate back to basic privelege */
            useraccess = BASICUSER;
        } else {
            if (strlen(args[0]) == strlen(PASSWORD) && !strncmp(args[0], PASSWORD, strlen(PASSWORD))) {
                useraccess = SUPERUSER;
            } else {
                HPUTS("Incorrect password.\n");
            }
        }
    } else {
        HPUTS("Usage: auth <password>\n");
    }
}
