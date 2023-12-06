#include <stdio.h>
#include <stdlib.h>

enum
{
    NUMOFNUMBERS = 10,
    NUMOFLETTERS = 26,
    CODESIZE = 6,
    MAILDOG = 0,
    HASHTAG = 63,
    MINNUM = 1,
    MAXNUM = MINNUM + NUMOFNUMBERS - 1,
    MINLOWCASE = MAXNUM + 1,
    MAXLOWCASE = MINLOWCASE + NUMOFLETTERS - 1,
    MINCAP = MAXLOWCASE + 1,
    MAXCAP = MINCAP + NUMOFLETTERS - 1,
    CHARBITS = 8,
    SECONDBITMASK = 251,
    THIRDBITNASK = 8
};

unsigned char
from_ascii (unsigned char sym) 
{
    if ('A' <= sym && sym <= 'Z') {
        return sym - 'A' + MINCAP;
    } else if ('a' <= sym && sym <= 'z') {
        return sym - 'a' + MINLOWCASE;
    } else if ('0' <= sym && sym <= '9') {
        return sym - '0' + MINNUM;
    } else {
        exit(1); //if given symbol is not correct
    }
}

unsigned char 
to_ascii (unsigned char sym)
{
    if (sym == MAILDOG) {
        return '@';
    } else if (sym == HASHTAG) {
        return '#';
    } else if (MINNUM <= sym && sym <= MAXNUM) {
        return sym - MINNUM + '0';
    } else if (MINCAP <= sym && sym <= MAXCAP) {
        return sym - MINCAP + 'A';
    } else if (MINLOWCASE <= sym && sym <= MAXLOWCASE) {
        return sym - MINLOWCASE + 'a';
    } else {
        exit(1); //if given symbol is not correct
    }
}

int 
main (void)
{
    int cur;
    while ((cur = getchar()) != EOF) {
        if (('0' <= cur && cur <= '9') || 
                ('A' <= cur && cur <= 'Z') || ('a' <= cur && cur <= 'z')) {
            unsigned char recoded = from_ascii((unsigned char)cur);
            recoded &= SECONDBITMASK; //nulify second bit
            recoded ^= THIRDBITNASK; // invert trird bit
            putchar(to_ascii(recoded));
        }
    }
    printf("\n");
}
