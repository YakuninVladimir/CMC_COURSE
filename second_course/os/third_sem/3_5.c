#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum
{
    INT_STR_SIZE = 12,
    SYS_BASE = 10
};

struct Elem
{
    struct Elem *next;
    char *str;
};

bool
is_el_int32(struct Elem *el)
{
    errno = 0;
    char *str = el->str;
    char *buf = NULL;
    long long int a = strtol(str, &buf, SYS_BASE);
    if (errno || *buf || buf == str || a >= INT_MAX || a < INT_MIN) {
        return false;
    }
    return true;
}

int
read_num(struct Elem *el)
{
    char *str = el->str;
    char *buf = NULL;
    return strtol(str, &buf, SYS_BASE);
}

struct Elem *
dup_elem(struct Elem *head)
{
    if (head == NULL) {
        return head;
    }
    struct Elem *ans = head;
    if (is_el_int32(head)) {
        struct Elem *new_struct = calloc(1, sizeof(struct Elem));
        int res = read_num(head);
        char buf[INT_STR_SIZE];
        sprintf(buf, "%d", res + 1);
        new_struct->str = strdup(buf);
        new_struct->next = head;
        ans = new_struct;
    }
    while (head->next != NULL) {
        if (is_el_int32(head->next)) {
            struct Elem *new_struct = calloc(1, sizeof(struct Elem));
            int res = read_num(head->next);
            char buf[INT_STR_SIZE];
            sprintf(buf, "%d", res + 1);
            new_struct->str = strdup(buf);
            new_struct->next = head->next;
            head->next = new_struct;
            head = new_struct->next;
        } else {
            head = head->next;
        }
    }
    return ans;
}
