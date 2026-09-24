#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_SIZE 3
#define PADDLE_LEFT_X 2
#define PADDLE_RIGHT_X (WIDTH - 3)
#define WIN_SCORE 21

void draw_game(int left_y, int right_y, int b_x, int b_y, int score_l, int score_r) {
    clear();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                mvaddch(y, x, '#');
            } else if (x == PADDLE_LEFT_X && y >= left_y && y < left_y + PADDLE_SIZE) {
                mvaddch(y, x, '|');
            } else if (x == PADDLE_RIGHT_X && y >= right_y && y < right_y + PADDLE_SIZE) {
                mvaddch(y, x, '|');
            } else if (x == b_x && y == b_y) {
                mvaddch(y, x, 'O');
            } else {
                mvaddch(y, x, ' ');
            }
        }
    }
    mvprintw(HEIGHT, 0, "Score Left %d Right %d | A Z (left) K M (right) quit - q", score_l, score_r);
    refresh();
}

int handle_input(int current_y, int ch, char up_key, char down_key) {
    int new_y = current_y;

    if (ch == up_key || ch == (up_key + 32)) {
        if (current_y > 1) {
            new_y--;
        }
    } else if (ch == down_key || ch == (down_key + 32)) {
        if (current_y < HEIGHT - PADDLE_SIZE - 1) {
            new_y++;
        }
    }
    return new_y;
}

void game_loop(void) {
    int left_paddle_y = HEIGHT / 2 - PADDLE_SIZE / 2;
    int right_paddle_y = HEIGHT / 2 - PADDLE_SIZE / 2;
    int ball_x = WIDTH / 2;
    int ball_y = HEIGHT / 2;
    int ball_dx = 1;
    int ball_dy = 1;
    int score_left = 0;
    int score_right = 0;
    int user_quit = 0;

    initscr();
    timeout(150);

    while (score_left < WIN_SCORE && score_right < WIN_SCORE && user_quit == 0) {
        draw_game(left_paddle_y, right_paddle_y, ball_x, ball_y, score_left, score_right);

        int ch = getch();

        if (ch == 'q' || ch == 'Q') {
            user_quit = 1;
        }

        if (user_quit == 0) {
            if (ch == 'a' || ch == 'A' || ch == 'z' || ch == 'Z') {
                left_paddle_y = handle_input(left_paddle_y, ch, 'A', 'Z');
            } else if (ch == 'k' || ch == 'K' || ch == 'm' || ch == 'M') {
                right_paddle_y = handle_input(right_paddle_y, ch, 'K', 'M');
            }

            ball_x += ball_dx;
            ball_y += ball_dy;

            if (ball_y <= 1 || ball_y >= HEIGHT - 2) {
                ball_dy = -ball_dy;
            }

            if (ball_x == PADDLE_LEFT_X + 1 && ball_y >= left_paddle_y &&
                ball_y < left_paddle_y + PADDLE_SIZE) {
                ball_dx = -ball_dx;
            }

            if (ball_x == PADDLE_RIGHT_X - 1 && ball_y >= right_paddle_y &&
                ball_y < right_paddle_y + PADDLE_SIZE) {
                ball_dx = -ball_dx;
            }

            if (ball_x <= 1) {
                score_right += 1;
                ball_x = WIDTH / 2;
                ball_y = HEIGHT / 2;
                ball_dx = 1;
                ball_dy = -ball_dy;
            } else if (ball_x >= WIDTH - 2) {
                score_left += 1;
                ball_x = WIDTH / 2;
                ball_y = HEIGHT / 2;
                ball_dx = -1;
                ball_dy = -ball_dy;
            }
        }
    }

    if (user_quit == 0) {
        draw_game(left_paddle_y, right_paddle_y, ball_x, ball_y, score_left, score_right);
        if (score_left >= WIN_SCORE) {
            mvprintw(HEIGHT + 1, 0, "Player 1 wins, press key to exit");
        } else {
            mvprintw(HEIGHT + 1, 0, "Player 2 wins, press key to exit");
        }
        refresh();
        timeout(-1);
        getch();
    }

    endwin();
}

int main(void) {
    game_loop();
    return 0;
}