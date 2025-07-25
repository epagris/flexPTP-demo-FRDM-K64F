int _close(int file) {
    (void)file;
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

__attribute__((weak)) int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;

    return len;
}

__attribute__((weak)) int _write(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;

    return len;
}
