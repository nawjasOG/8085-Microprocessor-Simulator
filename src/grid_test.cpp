#include <ncurses.h>
#include <string>

void draw_grid(WINDOW* win, int rows, int cols) {
    int height, width;
    getmaxyx(win, height, width);
    int cell_h = height/rows;
    int cell_w = width/cols;

    // horizontal internal lines
    for (int r = 0; r <= height; r += cell_h) {
        mvwhline(win, r, 1, ACS_HLINE, width-1);
    }

    // vertical internal lines
    for (int c = 0; c <= width; c += cell_w) {
        mvwvline(win, 1, c, ACS_VLINE, height-1);
    }

    for (int r = 0; r <= rows; ++r) {
        for (int c = 0; c <= cols; ++c) {
            int y = r * cell_h, x = c * cell_w;
            if (r == 0 && c == 0) mvwaddch(win, y, x, ACS_ULCORNER);
            else if (r == 0 && c == cols) mvwaddch(win, y, x, ACS_URCORNER);
            else if (r == rows && c == 0) mvwaddch(win, y, x, ACS_LLCORNER);
            else if (r == rows && c == cols) mvwaddch(win, y, x, ACS_LRCORNER);
            else if (r == 0) mvwaddch(win, y, x, ACS_TTEE);
            else if (c == 0) mvwaddch(win, y, x, ACS_LTEE);
            else if (r == rows) mvwaddch(win, y, x, ACS_BTEE);
            else if (c == cols) mvwaddch(win, y, x, ACS_RTEE);
            else mvwaddch(win, y, x, ACS_PLUS);
        }
    }
}

int main() {
    initscr();
    noecho();
    cbreak();
    refresh();

    int h = 25, w = 49;
    WINDOW* win = newwin(h, w, 1, 1);

    // follow this formula: (length-(rows+1))%rows == 0
    draw_grid(win, 12, 12);

    wrefresh(win);
    getch();
    endwin();
    return 0;
}
