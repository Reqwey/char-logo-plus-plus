/*
*   @author Linhk1606 Linhk1606@outlook.com
*   @version 1.1
*/

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <map>

#define row(x) ques[x].row
#define col(x) ques[x].col
#define llpr(x) ques[x].left_len_per_row
#define rlpr(x) ques[x].right_len_per_row
#define pic(x) ques[x].pic

using namespace std;

const int MAX_INPUT = 201, MAX_CANVAS = 1001, MAX_CHAR = 21, BLANK_SIZE = 3,INF = 0x7ffffff;

struct myChar
{
    int row, col;
    int left_len_per_row[MAX_CHAR], right_len_per_row[MAX_CHAR];
    char pic[MAX_CHAR][MAX_CHAR];
} ques[MAX_INPUT];

char str[MAX_INPUT], canvas[MAX_CANVAS][MAX_CANVAS];

int canvas_x, canvas_y;

map<char, string> find_file;

inline bool is_normal_char(char c)
{
    return (isalpha(c) || isdigit(c));
}

void init_map()
{
    string tmp_A, tmp_a, tmp_num;
    for (int i = 0; i < 26; i++) // 大小写字母
    {
        tmp_A.push_back('A' + i), tmp_a.push_back('a' + i); // char 转 string
        find_file[tmp_A[0]] = tmp_A;
        find_file[tmp_a[0]] = tmp_a;
        tmp_A.pop_back(), tmp_a.pop_back();
    }
    for (int i = 0; i <= 9; i++) // 数字
    {
        tmp_num.push_back('0' + i); // char 转 string
        find_file[tmp_num[0]] = tmp_num;
        tmp_num.pop_back();
    }
    find_file['!'] = "exclamation";
    find_file['?'] = "question";
    find_file[':'] = "colon";
    find_file[';'] = "semicolon";
    find_file[','] = "comma";
    find_file['.'] = "period";
    find_file['@'] = "at";
    find_file['#'] = "num";
    find_file['$'] = "dollar";
    find_file['%'] = "percent";
    find_file['^'] = "up";
    find_file['&'] = "and";
    find_file['|'] = "or";
    find_file['*'] = "star";
}

bool init_char(int ii)
{
    FILE *fp = NULL;
    char file_path[MAX_INPUT];
    sprintf(file_path, "./ascii/%s.log", find_file[str[ii]].c_str());
    // system(file_path);
    if ((fp = fopen(file_path, "r")) == NULL)
    {
        printf("Open filename fail...Command is %s", file_path);
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
    for (int i = 0, j = 0; i < row(ii) && j < row(jj); i++, j++)
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
    init_map();
    puts("CharLogo++ - A simple tool to generate character letter banners\n"
         "Usage: Input characters for conversion. Type ! to quit");
    while (true)
    {
        bool try_again = false;
        printf(">>>");
        memset(str, 0, sizeof(str));
        cin.getline(str, MAX_INPUT - 2);
        if (strlen(str) == 1 && str[0] == '!')
            break;
        for (int i = 0; i < strlen(str); i++)
        {
            if (find_file.find(str[i]) == find_file.end() && str[i] != ' ')
            {
                try_again = true;
                break;
            }
        }
        if (try_again)
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
                if (str[i] != ' ')
                    if (init_char(i))
                    {
                        file_error = true;
                        break;
                    }
            if (file_error)
                break;
            if (str[0] != ' ')
                draw_char(0, 0, canvas_y, true);
            for (int i = 1; i < strlen(str); i++)
            {
                if (str[i] != ' ')
                {
                    if (str[i - 1] != ' ')
                    {
                        bool flag_i_1 = is_normal_char(str[i - 1]), flag_i = is_normal_char(str[i]);
                        if (flag_i_1 && flag_i) // 前后都是字母/数字
                        {
                            canvas_y += col(i - 1) - calc_dist(i - 1, i) - 1;
                            draw_char(i, 0, canvas_y, false);
                        }
                        else
                        {
                            canvas_y += col(i - 1);
                            draw_char(i, 0, canvas_y, true); // 不想和其它字符粘在一块～
                        }
                    }
                    else
                    {
                        canvas_y += BLANK_SIZE;
                        draw_char(i, 0, canvas_y, true);
                    }
                }
                else
                {
                    if (str[i - 1] != ' ')
                        canvas_y += col(i - 1);
                    else
                        canvas_y += BLANK_SIZE;
                }
            }
            if (str[strlen(str) - 1] != ' ')
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
