#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char *
strcpy(char *s, const char *t) {
    char *os;

    os = s;
    while ((*s++ = *t++) != 0);
    return os;
}

int
strcmp(const char *p, const char *q) {
    while (*p && *p == *q)
        p++, q++;
    return (uchar) *p - (uchar) *q;
}

uint
strlen(const char *s) {
    int n;

    for (n = 0; s[n]; n++);
    return n;
}

void *
memset(void *dst, int c, uint n) {
    stosb(dst, c, n);
    return dst;
}

char *
strchr(const char *s, char c) {
    for (; *s; s++)
        if (*s == c)
            return (char *) s;
    return 0;
}

// Concatenate two strings
char *
my_strcat(char* destination, const char* source) {
    int i, j;

    // move to the end of destination string
    for (i = 0; destination[i] != '\0'; i++);

    // i now points to terminating null character in destination

    // Appends characters of source to the destination string
    for (j = 0; source[j] != '\0'; j++)
        destination[i + j] = source[j];

    // null terminate destination string
    destination[i + j] = '\0';

    // destination is returned by standard strcat()
    return destination;
}

char *
gets(char *buf, int max) {
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;) {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        buf[i++] = c;
        if (c == '\n' || c == '\r')
            break;
    }
    buf[i] = '\0';
    return buf;
}

int
stat(const char *n, struct stat *st) {
    int fd;
    int r;

    fd = open(n, O_RDONLY);
    if (fd < 0)
        return -1;
    r = fstat(fd, st);
    close(fd);
    return r;
}

int
atoi(const char *s) {
    int n;

    n = 0;
    while ('0' <= *s && *s <= '9')
        n = n * 10 + *s++ - '0';
    return n;
}

void *
memmove(void *vdst, const void *vsrc, int n) {
    char *dst;
    const char *src;

    dst = vdst;
    src = vsrc;
    while (n-- > 0)
        *dst++ = *src++;
    return vdst;
}
