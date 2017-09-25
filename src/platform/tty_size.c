#include "moar.h"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
#endif
MVMint64 * MVM_io_tty_size_platform (MVMThreadContext *tc, MVMOSHandle *h) {
    MVMint64 *result = alloca(2 * sizeof(MVMint64));
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    rows    = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
#else
    struct winsize w;
    int columns, rows;
    ioctl(data->fd, TIOCGWINSZ, &w);
    columns = w.ws_col;
    rows    = w.ws_row;
#endif
    printf("columns: %d\n", columns);
    printf("rows: %d\n", rows);
    result[0] = columns;
    result[1] = rows;
    return result;
}
