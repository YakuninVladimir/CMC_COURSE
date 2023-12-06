#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
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
create_elem(struct Elem *head)
{
    errno = 0;
    struct Elem *new_struct = calloc(1, sizeof(*head));
    if (new_struct == NULL) {
        fprintf(stderr, "Error, can't allocate memory: %s", strerror(errno));
        exit(1);
    }
    int res = read_num(head);
    char *buf = NULL;
    asprintf(&buf, "%d", res + 1);
    new_struct->str = strdup(buf);
    free(buf);
    new_struct->next = head;
    return new_struct;
}

struct Elem *
dup_elem(struct Elem *head)
{
    if (head == NULL) {
        return head;
    }
    struct Elem *ans = head;
    if (is_el_int32(head)) {
        struct Elem *new_struct = create_elem(head);
        ans = new_struct;
    }
    while (head->next != NULL) {
        if (is_el_int32(head->next)) {
            struct Elem *new_struct = create_elem(head->next);
            head->next = new_struct;
            head = new_struct->next;
        } else {
            head = head->next;
        }
    }
    return ans;
}
