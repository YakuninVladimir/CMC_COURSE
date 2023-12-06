#include <malloc.h>
#include <string.h>

void
normalize_path(char *buf)
{
    int n = 0;
    while (buf[n] != '\0') {
        n++;
    }
    int j = 0, i = 0;
    while (i < n) {
        int flag = 0;
        if (buf[i] == '/' && buf[i + 1] == '.') {
            buf[j] = '/';
            if (i + 2 < n && buf[i + 2] == '.' && (buf[i + 3] == '/' || buf[i + 3] == '\0')) {
                flag = 1;
                i += 3;
                int slashes = 0;
                while (j >= 0 && slashes < 2) {
                    if (buf[j] == '/') {
                        slashes++;
                    }
                    j--;
                }
                j++;
            } else if (buf[i + 2] == '\0' || buf[i + 2] == '/') {
                flag = 1;
                i += 2;
            }
        }
        if (!flag) {
            buf[j] = buf[i];
            i++;
            j++;
        }
    }
    if (j == 0) {
        buf[j] = '/';
        buf[j + 1] = '\0';
    } else if (j > 1 && buf[j - 1] == '/') {
        buf[j - 1] = '\0';
    } else {
        buf[j] = '\0';
    }
}

char *
relativize_path(const char *path1, const char *path2)
{
    char temp[] = "../";
    char *npath1 = strdup(path1);
    char *npath2 = strdup(path2);
    normalize_path(npath1);
    normalize_path(npath2);
    int len1 = strlen(npath1);
    int len2 = strlen(npath2);
    char *res = calloc(sizeof(*res), 2 * (len1 + len2) + 1);
    int s_len = 0;
    int slash1 = len1 - 1;
    while (npath1[slash1] != '/') {
        slash1--;
    }
    for (int i = 0; i <= slash1; i++) {
        if (npath1[i] == '/') {
            s_len = i;
        }
        if (npath1[i] != npath2[i]) {
            if ((npath1[i] == 0 && npath2[i] == '/') || (npath1[i] == '/' && npath2[i] == 0)) {
                s_len = i;
            }
            break;
        }
    }

    int cur = 0;
    for (int i = s_len; i < slash1; i++) {
        if (npath1[i] == '/') {
            for (int q = 0; q < sizeof(temp) - 1; q++) {
                res[cur + q] = temp[q];
            }
            cur += sizeof(temp) - 1;
        }
    }

    for (int i = s_len + 1; i <= len2; i++) {
        res[cur] = npath2[i];
        cur++;
    }
    int res_n = strlen(res);
    if (res_n != 0 && res[res_n - 1] == '/') {
        res[res_n - 1] = '\0';
    }
    if (cur == 1 || cur == 0) {
        res[0] = '.';
    }
    free(npath1);
    free(npath2);
    return res;
}
