/************************命令行版贪吃蛇***********************/
/*本游戏在Windows环境下开发,Linux暂不支持*/
/*本游戏采用多线程设计，主线程主要用于监听键盘操作，次线程主要控制蛇前进*/
/*cpu占用率在双核core i5上保持在0.7%以下，四核core i7基本保持在0.1%以下，内存占用0.4M*/
/*本游戏通过windows下的gcc 4.9.3和vs2015的编译，
使用vs编译时请包含头文件"stdafx.h"，并将第53行的getch函数修改为_gerch*/
//time:2016-04-24

#include "stdafx.h" //for vs2015！！！！
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <cmath>
#include <windows.h>

using namespace std;

/************************函数声明区************************/
int init(); //初始化函数
int wall(); //墙壁
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;
COORD pos = {0, 0}; //定位光标原点
int location(int x, int y); //二维平面光标定位
int hide(); //隐藏光标
int food(); //随机输出食物
DWORD WINAPI snake(LPVOID lpParameter); //蛇，次线程
bool eat();//判断是否吃到食物
bool condition_fun();//让游戏继续的条件
int Score();//分数
/**************************结束**************************/


/***********************变量初始化***********************/
struct coordinate { //坐标结构体
    int x, y;
} food_coord = {10, 5}, snake_coord[250], temp[250]; //蛇是一个字符数组，最长为250

int min_x = 1, max_x = 25; //墙宽范围
int min_y = 1, max_y = 10; //墙高范围
int snake_len = 5;//初始化蛇长为5
int sleep_time = 250;//初始运动时间间隔，250ms
int score = 0;//得分
char direct = 77;//蛇默认向右运动
/**************************结束*************************/


int main() { //main，主线程
    char raw_dire;
    init();
    HANDLE sThread = CreateThread(NULL, 0, snake, NULL, 0, NULL);
    CloseHandle(sThread);//创建蛇运动的线程
    while (1) {
        raw_dire = _getch();//监听键盘按键(vs下编译请用 _getch)！！！
        if (direct == 72 || direct == 80) { //不准反方向运动
            if (raw_dire == 75 || raw_dire == 77)
                direct = raw_dire;
            else
                continue;
        }
        else {
            if (raw_dire == 72 || raw_dire == 80)
                direct = raw_dire;
            else
                continue;
        }
    }
    return 0;
}


int init() {
    system("title GreedSnake");//标题
    system("cls");//清屏
    hide();
    wall();
    snake_coord[0].x = 5, snake_coord[0].y = 1; //初始化蛇坐标
    location(5, 1);
    cout << "@";
    for (int i = snake_len - 1, j = 1; i > 0; i--, j++) {
        location(i, 1);
        snake_coord[j].x = i;
        snake_coord[j].y = 1;
        cout << "*";
    }
    location(food_coord.x, food_coord.y); //第一次输出食物
    cout << "&";
    Score();
    return 0;
}


int location(int x, int y) { //光标定位
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hout, coord);
    return 0;
}


int hide() { //隐藏光标
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(hout, &cursor_info);
    return 0;
}


int wall() {
    cout << "+";
    for (int i = 1; i <= max_x; i++) //输出上边界
        cout << "-";
    cout << "+" << endl;
    for (int j = 0; j <= max_y - 1; j++) {
        cout << "|"; //输出左边界
        for (int i = 1; i <= max_x; i++) cout << " ";
        cout << "|" << endl; //输出右边界
    }
    cout << "+";
    for (int i = 1; i <= max_x; i++)
        cout << "-"; //输出下边界
    cout << "+" << endl;
    return 0;
}


int food() {
    food_coord.x = ((rand() % (max_x - min_x + 1)) + min_x); //输出[min_x,max_x]范围内的随机数
    food_coord.y = ((rand() % (max_y - min_y + 1)) + min_y); //输出[min_y,max_y]范围内的随机数
    location(food_coord.x, food_coord.y);
    cout << "&";
    return 0;
}


DWORD WINAPI snake(LPVOID lpParameter) {
    while (1) {
        bool condition = condition_fun();
        if (!condition) {
            system("cls");
            cout << "GAMEOVER!" << endl;
            cout << "SCORE:" << score;
            break;
        }
        for (int i = 0; i < snake_len; i++) { //保存蛇原来的坐标
            temp[i] = snake_coord[i];
        }
        switch (direct) {
        case 72: {
            snake_coord[0].y -= 1;//向上运动
            break;
        }
        case 80: {
            snake_coord[0].y += 1;//向下运动
            break;
        }
        case 75: {
            snake_coord[0].x -= 1;//向左运动
            break;
        }
        case 77: {
            snake_coord[0].x += 1;//向右运动
            break;
        }
        default: break;
        }
        location(snake_coord[0].x, snake_coord[0].y);
        cout << "@";

        for (int j = 0; j < snake_len - 1; j++) { //还原坐标
            snake_coord[j + 1] = temp[j];
        }
        for (int k = 1; k < snake_len; k++) { //清空原来的蛇体
            location(temp[k].x, temp[k].y);
            cout << " ";
        }
        for (int l = 1; l < snake_len; l++) { //根据现有坐标重绘蛇体
            location(snake_coord[l].x, snake_coord[l].y);
            cout << "*";
        }
        eat();
        Sleep(sleep_time);//暂停该线程
    }
    return 0;
}


bool eat() {
    if ((snake_coord[0].x == food_coord.x) && (snake_coord[0].y == food_coord.y)) { //吃到食物
        sleep_time -= 3;//间隔时间减3ms
        snake_len += 1;//蛇长加一
        score += 1;//得分加一
        food();
        for (int i = 0; i < snake_len; i++) { //检查食物是否出现在蛇身上
            if (snake_coord[i].x == food_coord.x && snake_coord[i].y == food_coord.y)
                food();//如果出现在蛇上，则重新生成
            else
                continue;
        }
        Score();
        return true;
    }
    else
        return false;
}


int Score() {
    location(1, 12);
    cout << "Score:" << score;
    return 0;
}


bool condition_fun() {
    coordinate temp = snake_coord[0];
    if (temp.x == 0 || temp.x == 26 || temp.y == 0 || temp.y == 11) //碰壁
        return false;
    for (int i = 1; i < snake_len; i++) { //自残
        if (temp.x == snake_coord[i].x && temp.y == snake_coord[i].y)
            return false;
    }
    return true;
}