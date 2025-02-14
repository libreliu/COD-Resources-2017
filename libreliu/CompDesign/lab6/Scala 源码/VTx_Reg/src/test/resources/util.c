

int cursor_h = 0;
int cursor_v = 0;

void putchar(int ch);
int getchar();
void clear_scr();
void backspace();

void main_loop() {
    cursor_h = 0;
    cursor_v = 0;
    int ch;
    //clear_scr();
    putchar('>' | 0xF00);
    for (;;) {
        // putchar('_' | 0x700);
        ch = getchar();

        if (ch == '\n' || ch == '\r') {
            cursor_h = 0;
            cursor_v += 1;
        } else if (ch == 8) {
            // Backspace
            if (cursor_h == 0) {
                cursor_h = 24;
                cursor_v -= 1;
                putchar(' ' | 0xF00);
                cursor_h = 24;
                cursor_v -= 1;
            } else {
                cursor_h -= 1;
                putchar(' ' | 0xF00);
                cursor_h -= 1;
            }
        } else if (ch == 27) {
            for (int i = 0; i < 4000; i++) {
                putchar(' ' | 0xF00);
            }
            cursor_h = 0;
            cursor_v = 0;
        } else {
            putchar(ch | 0xF00);
        }
    }
}

// void backspace() {
//     if (cursor_h == 0) {
//         cursor_h = 24;
//         cursor_v -= 1;
//         putchar(' ' | 0xF00);
//         cursor_h = 24;
//         cursor_v -= 1;
//     } else {
//         cursor_h -= 1;
//         putchar(' ' | 0xF00);
//         cursor_h -= 1;
//     }
// }

// void clear_scr() {

// }

void putchar(int ch) {
    volatile int *vram_offset = 0x3000;
    vram_offset[cursor_v * 80 + cursor_h] = ch;
    cursor_h++;
    if (cursor_h == 80) {
        cursor_h = 0;
        cursor_v++;
        if (cursor_v == 25) {
            cursor_v = 0;
        }
    }
}

int getchar() {
    // uart offset
    volatile int *uart_offset = 0xFF00;
    int valid_mask = 0x40000000;
    int byte_mask = 0x000000ff;
    while ((*uart_offset & valid_mask) == 0) {
        // Ready; do nothing
        //__asm__("nop");
    }
    int ret = byte_mask & *uart_offset;
    *uart_offset = (1 << 31);
    return ret;
}

