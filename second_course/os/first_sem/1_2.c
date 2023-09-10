#include <stdio.h>
#include <stdlib.h>

enum
{
    CODESIZE = 6,
    MAILDOG = 0,
    HASHTAG = 63,
    MINNUM = 1,
    MAXNUM = 10,
    MINLOWCASE = 11,
    MAXLOWCASE = 36,
    MINCAP = 37,
    MAXCAP = 62,
    CHARBITS = 8
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

unsigned char SECONDBITMASK = 251;
unsigned char THIRDBITNASK = 8;

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
