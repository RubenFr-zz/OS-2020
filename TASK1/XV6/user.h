struct stat;
struct rtcdate;

// system calls
int fork(void);

//int exit(void) __attribute__((noreturn));

int exit(int status) __attribute__((noreturn)); // TASK 4.1

//int wait(void);

int wait(int *status);  // TASK 4.2

int pipe(int *);

int write(int, const void *, int);

int read(int, void *, int);

int close(int);

int kill(int);

int exec(char *, char **);

int open(const char *, int);

int mknod(const char *, short, short);

int unlink(const char *);

int fstat(int fd, struct stat *);

int link(const char *, const char *);

int mkdir(const char *);

int chdir(const char *);

int dup(int);

int getpid(void);

char *sbrk(int);

int sleep(int);

int uptime(void);

int getinfo(void); // Task 3.2

// ulib.c
int stat(const char *, struct stat *);

char *strcpy(char *, const char *);

void *memmove(void *, const void *, int);

char *strchr(const char *, char c);

char *my_strcat(char* destination, const char* source);

int strcmp(const char *, const char *);

void printf(int, const char *, ...);

char *gets(char *, int max);

uint strlen(const char *);

void *memset(void *, int, uint);

void *malloc(uint);

void free(void *);

int atoi(const char *);

// sh.c
void new_path(char* path);

void print_PATH();