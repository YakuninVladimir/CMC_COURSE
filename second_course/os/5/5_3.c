#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }
    const char FORMAT[] = "rwxrwxrwx";
    int form_s = sizeof(FORMAT) - 1, str_s = strlen(str);
    if (form_s != str_s) {
        return -1;
    }

    unsigned mask = 1 << (form_s - 1);
    unsigned rights = 0;
    for (int i = 0; i < form_s; i++) {
        if (str[i] == FORMAT[i]) {
            rights += mask;
        } else if (str[i] != '-') {
            return -1;
        }
        mask >>= 1;
    }
    return rights;
}
