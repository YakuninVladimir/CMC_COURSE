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
