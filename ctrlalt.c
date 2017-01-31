#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static int doctrl(int c)
{
    return c & 0x1f;
}

static int doalt(int c)
{
    return c + 128;
}

static int doshift(int c)
{
    return c ^ 32;
}

/* Codes:
 *    ^X: Ctrl  X
 *   C-X: Ctrl  X
 *   M-X: Alt   X
 *   S-X: Shift X
 * X can be:
 *     C: Any character
 *        (single digits are the character, not the value)
 *   0dd: Octal value
 *  0xdd: Hex value
 *   ddd: Decimal value
 * Return values:
 *   -1:  Error
 * else:  Result of applying the code
 */
static int parse(char *str)
{
    size_t len;
    int res = -1;
    bool ctrl = false, alt = false, shift = false;

    if ((str[0] >= '0') && (str[0] <= '9')) {
        return strtol(str, NULL, 0);
    }

    len = strlen(str);

    switch (len) {
    case 0:
        return -1;
        break;
    case 1:
        return str[0];
        break;
    case 2:
        if ('^' == str[0]) {
            return doctrl(str[1]);
        } else {
            return -1;
        }

        break;
    default:
        if ('^' == str[0]) {
            ctrl = true;
            str += 1;
        } else if ('-' == str[1]) {
            if (str[0] == 'C') {
                ctrl = true;
            } else if (str[0] == 'S') {
                shift = true;
            } else if (str[0] == 'M') {
                alt = true;
            } else {
                return -1;
            }

            str += 2;
        } else {
            return -1;
        }

        break;
    }

    if ((len > 3) && (str[0] >= '0') && (str[0] <= '9')) {
        res = strtol(str, NULL, 0);
    } else if (len > 3) {
        return -1;
    } else {
        res = str[0];
    }

    if (ctrl) {
        return doctrl(res);
    } else if (shift) {
        return doshift(res);
    } else if (alt) {
        return doalt(res);
    } else {
        return res;
    }
}

static void help(void)
{
    puts("Usage: ctrlalt [CODE]...");
    puts("Print the results of applying various ASCII codes to characters");
    puts("");
    puts("Codes:");
    puts("   ^X: Ctrl  X");
    puts("  C-X: Ctrl  X");
    puts("  M-X: Alt   X");
    puts("  S-X: Shift X");
    puts("X can be:");
    puts("    C: Any character");
    puts("       (single digits are the character, not the value)");
    puts("  0dd: Octal value");
    puts(" 0xdd: Hex value");
    puts("  ddd: Decimal value");
    puts("");
    puts("Report bugs to <chbarts@gmail.com>.");
}

static void version(void)
{
    puts("ctrlalt 1.0");
    puts("This software is in the public domain.");
    puts("Chris Barts, 2017");
}

int main(int argc, char *argv[])
{
    int i, res;

    if (1 == argc) {
        help();
        return 0;
    }

    if ((2 == argc) && ('-' == argv[1][0])) {
        if ('\0' == argv[1][1]) {
            putchar('-');
            return 0;
        } else if ((!strcmp(argv[1], "--help"))
                   || (!strcmp(argv[1], "-h"))) {
            help();
            return 0;
        } else if ((!strcmp(argv[1], "--version"))
                   || (!strcmp(argv[1], "-v"))) {
            version();
            return 0;
        } else {
            fprintf(stderr, "%s: invalid code %s\n", argv[0], argv[1]);
            help();
            exit(EXIT_FAILURE);
        }
    }

    for (i = 1; i < argc; i++) {
        if ((res = parse(argv[i])) == -1) {
            fprintf(stderr, "%s: invalid code %s", argv[0], argv[i]);
            continue;
        }

        putchar(res);
    }

    return 0;
}
