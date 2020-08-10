//
// Created by linhk on 20-8-9.
//

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>

#define row(x) ques[x].row
#define col(x) ques[x].col
#define llpr(x) ques[x].left_len_per_row
#define rlpr(x) ques[x].right_len_per_row
#define pic(x) ques[x].pic

using namespace std;

const int MAX_INPUT = 201, MAX_CANVAS = 1001, MAX_CHAR = 21, INF = 0x7ffffff;

struct myChar {
    int row, col;
    int left_len_per_row[MAX_CHAR], right_len_per_row[MAX_CHAR];
    char pic[MAX_CHAR][MAX_CHAR];
} ques[MAX_INPUT];

char str[MAX_INPUT], canvas[MAX_CANVAS][MAX_CANVAS];

int canvas_x, canvas_y;

void init_char(int ii) {
    char command[MAX_INPUT];
    sprintf(command, "./ascii/%c.log", str[ii]);
//    system(command);
    freopen(command, "r", stdin);
    row(ii) = col(ii) = 0;
    while (cin.getline(pic(ii)[row(ii)], MAX_CHAR - 5)) {
        int tmp = strlen(pic(ii)[row(ii)]);
        col(ii) = max(col(ii), tmp);
        row(ii)++;
    }
    canvas_x = max(canvas_x, row(ii));
    for (int i = 0; i < row(ii); i++) {
        for (int j = strlen(pic(ii)[i]); j < col(ii); j++)
            pic(ii)[i][j] = ' ';
        llpr(ii)[i] = 0, rlpr(ii)[i] = col(ii) - 1;
        for (; llpr(ii)[i] < col(ii) && pic(ii)[i][llpr(ii)[i]] == ' '; llpr(ii)[i]++);
        for (; rlpr(ii)[i] >= 0 && pic(ii)[i][rlpr(ii)[i]] == ' '; rlpr(ii)[i]--);
    }
    fclose(stdin);
    cin.clear();
}

int calc_dist(int ii, int jj) {
    int min_dist = INF;
    for (int i = row(ii) - 1, j = row(jj) - 1; i >= 0 && j >= 0; i--, j--)
        min_dist = min(min_dist, rlpr(ii)[i] + llpr(jj)[j]);
    return min_dist;
}

void draw_char(int ii, int x, int y) {
    // 第一列与上一个字母重合, 起到连续效果, 须单独处理
    for (int i = 0; i < row(ii); i++) {
        if (canvas[x + i][y] == pic(ii)[i][0])
            canvas[x + i][y] = ' ';
        else
            canvas[x + i][y] = pic(ii)[i][0];
    }
    for (int i = 0; i < row(ii); i++)
        for (int j = 1; j < col(ii); j++)
            canvas[x + i][y + j] = pic(ii)[i][j];
}

int main() {
    system("ls");
    puts("CharLogo++ - A simple tool to generate character letter banners\n"
         "Usage: Input English characters(a to z, A to Z) for conversion. Type ! to quit");
    while (true) {
        bool exit = false, try_again = false;
        printf(">>>");
        memset(str, 0, sizeof(str));
        scanf("%s", str);
        printf("is \"%s\"\n", str);
        for (int i = 0; i < strlen(str); i++) {
            if (!isalpha(str[i])) {
                if (str[i] == '!') {
                    exit = true;
                    break;
                } else {
                    try_again = true;
                    break;
                }
            }
        }
        if (exit)
            break;
        else if (try_again)
            continue;
        else {
            memset(ques, 0, sizeof(ques));
            memset(canvas, 0, sizeof(canvas));
            canvas_x = canvas_y = 0;
            for (int i = 0; i < strlen(str); i++)
                init_char(i);
            draw_char(0, canvas_x - row(0), canvas_y);
            for (int i = 1; i < strlen(str); i++) {
                canvas_y += col(i - 1) - calc_dist(i - 1, i) - 1;
                draw_char(i, canvas_x - row(i), canvas_y);
            }
            canvas_y += col(strlen(str) - 1);
            for (int i = 0; i < canvas_x; i++) {
                for (int j = 0; j < canvas_y; j++) {
                    putchar(canvas[i][j]);
                }
                puts("");
            }
        }
    }
    return 0;
}
