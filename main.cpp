/*
*   @author Linhk1606
*   @version 1.0.0 还没来得及写注释版
*/

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

struct myChar
{
    int row, col;
    int left_len_per_row[MAX_CHAR], right_len_per_row[MAX_CHAR];
    char pic[MAX_CHAR][MAX_CHAR];
} ques[MAX_INPUT];

char str[MAX_INPUT], canvas[MAX_CANVAS][MAX_CANVAS];

int canvas_x, canvas_y;

bool init_char(int ii)
{
    FILE *fp = NULL;
    char file_path[MAX_INPUT];
    sprintf(file_path, "./ascii/%c.log", str[ii]);
    // system(file_path);
    if ((fp = fopen(file_path, "r")) == NULL)
    {
        puts("Open filename fail...");
        return 1;
    }

    row(ii) = col(ii) = 0;

    while (fgets(pic(ii)[row(ii)], MAX_CHAR - 2, fp) != NULL)
    {
        char *findChar = strchr(pic(ii)[row(ii)], '\n'); //查找换行符
        if (findChar)                                    //如果地址不是NULL
            *findChar = '\0';                            //在此处放置一个空字符
        int tmp = strlen(pic(ii)[row(ii)]);
        col(ii) = max(col(ii), tmp);
        row(ii)++;
    }
    canvas_x = max(canvas_x, row(ii));
    for (int i = 0; i < row(ii); i++)
    {
        for (int j = strlen(pic(ii)[i]); j < col(ii); j++)
            pic(ii)[i][j] = ' ';
        llpr(ii)[i] = 0, rlpr(ii)[i] = col(ii) - 1;
        for (; llpr(ii)[i] < col(ii) && pic(ii)[i][llpr(ii)[i]] == ' '; llpr(ii)[i]++);
        for (; rlpr(ii)[i] >= 0 && pic(ii)[i][rlpr(ii)[i]] == ' '; rlpr(ii)[i]--);
        rlpr(ii)[i] = col(ii) - rlpr(ii)[i] - 1;
    }
    fclose(fp);
    return 0;
}

int calc_dist(int ii, int jj)
{
    int min_dist = INF;
    for (int i = row(ii) - 1, j = row(jj) - 1; i >= 0 && j >= 0; i--, j--)
        min_dist = min(min_dist, rlpr(ii)[i] + llpr(jj)[j]);
    return min_dist;
}

void draw_char(int ii, int x, int y, bool bFirst)
{
    // 第一列与上一个字母重合, 起到连续效果, 须单独处理
    for (int i = 0; i < row(ii); i++)
        if (pic(ii)[i][0] != ' ' || bFirst)
            canvas[x + i][y] = pic(ii)[i][0];
    for (int i = 0; i < row(ii); i++)
        for (int j = 1; j < col(ii); j++)
            canvas[x + i][y + j] = pic(ii)[i][j];
}

int main()
{
    puts("CharLogo++ - A simple tool to generate character letter banners\n"
         "Usage: Input English characters(a to z, A to Z) for conversion. Type ! to quit");
    while (true)
    {
        bool exit = false, try_again = false;
        printf(">>>");
        memset(str, 0, sizeof(str));
        scanf("%s", str);
        for (int i = 0; i < strlen(str); i++)
        {
            if (!isalpha(str[i]))
            {
                if (str[i] == '!')
                {
                    exit = true;
                    break;
                }
                else
                {
                    try_again = true;
                    break;
                }
            }
        }
        if (exit)
            break;
        else if (try_again)
        {
            puts("Invalid characters");
            continue;
        }
        else
        {
            bool file_error = false;
            memset(ques, 0, sizeof(ques));
            memset(canvas, 0, sizeof(canvas));
            canvas_x = canvas_y = 0;
            for (int i = 0; i < strlen(str); i++)
                if (init_char(i))
                {
                    file_error = true;
                    break;
                }
            if (file_error)
                break;
            draw_char(0, canvas_x - row(0), canvas_y, true);
            for (int i = 1; i < strlen(str); i++)
            {
                canvas_y += col(i - 1) - calc_dist(i - 1, i) - 1;
                draw_char(i, canvas_x - row(i), canvas_y, false);
            }
            canvas_y += col(strlen(str) - 1);
            for (int i = 0; i < canvas_x; i++)
            {
                for (int j = 0; j < canvas_y; j++)
                {
                    if (canvas[i][j] == '\0')
                        putchar(' ');
                    else
                        putchar(canvas[i][j]);
                }
                puts("");
            }
        }
    }
    return 0;
}
