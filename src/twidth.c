#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    struct winsize w;

    // Get terminal size
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return 1;
    }

    printf("Terminal width: %d columns\n", w.ws_col);
    printf("Terminal height: %d rows\n", w.ws_row);

    for (int i = 0; i < w.ws_col / 2; i++) putchar(' ');
    putchar('.');
    putchar('\n');

    return 0;
}
