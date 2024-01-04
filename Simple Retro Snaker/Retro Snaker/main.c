#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>

//1. 设置地图边界
#define W 60
#define H 20

//2.设置贪吃蛇结构体
typedef struct _body
{
	int x;  //坐标x
	int y;  //坐标y
}BODY;

typedef struct snake
{
	BODY list[W * H];  //贪吃蛇最大占满整个地图
	int size;          // 实际身体个数;蛇头(@)和蛇身蛇尾(*)
	BODY food;         // 食物；#
	COORD coord;		//（定位光标）坐标	Window.h头文件中定义了；
	int dx;				//移动方向	右1 左-1		
	int dy;				//移动方向	上1 下-1
	BODY tail;          //记录蛇尾（用于清除蛇尾痕迹）
	int score;			//游戏分数；

}SNAKE;

//3.实现显示地图函数，初始化蛇的函数，初始化食物函数


void test_ui()
{
	for(int i = 0 ; i < H ; i++)
	{
		for( int j = 0 ; j < W ; j++)
		{
			printf("=");
		}
		printf("\n");
	}
	
}

void init_food(BODY* food)
{
	// 设置随机数种子
	srand(time(NULL));
	// 设置食物坐标
	food->x = rand() % (W-1) +1;//食物的坐标x在60以内
	food->y = rand() % (H-1) +1;//食物的坐标y在20以内
}

void init_snake(SNAKE* snake)
{
	//设置蛇头和蛇尾位置
	snake->list[0].x = W / 2;
	snake->list[0].y = H / 2;
	snake->list[1].x = W / 2 - 1;
	snake->list[1].x = H / 2 - 1;
	snake->size = 2;

	//设置移动的方向
	snake->dx = 1;	//默认设置移动方向是向右
	snake->dy = 0;

	snake->score = 0; //默认分数是0分

	// 设置食物位置
	init_food(&(snake->food));

}

void show_ui(SNAKE* snake)
{
	//显示蛇
	for(int i = 0; i < snake->size;i++)
	{
		snake->coord.X = snake->list[i]. x;
		snake->coord.Y = snake->list[i]. y;
		//定位光标的位置
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
		if (i == 0) //显示蛇头@
		{
			printf("@");
		}
		else     //显示蛇身和蛇尾
		{
			printf("*");
		}
		
	}

	//显示食物
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	//定位光标的位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("#");

	//清除蛇尾痕迹
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	//定位光标的位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");
}

void show_wall()
{ 
	for(int i = 0; i <= H; i++)
	{
		for(int j = 0;j <= W; j++)
		{
			if(i == 0 || j == 0 || i == H || j == W)
			{
				printf("+");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void hide_cur() //隐藏光标
{
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void move_snake(SNAKE * snake)
{
	//记录蛇尾（用于清除痕迹）
	snake->tail = snake->list[snake->size - 1]; 
	//蛇的移动是从后面开始移动：蛇头要单独设置要移动方向
	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i - 1];   // 2 = 1 , 1 = 0
	}
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}

void contorl_snake(SNAKE* snake)
{
	char  key = 0;
	while (_kbhit()) //判断是否按下按键,按下不等于0 //检测到按下按键事件
	{
		key = _getch();//读取按下的键值；
	}
	//抬起按下动作 或者 没有按下动作

	//根据按键修改移动方向
	switch (key)
	{
	case'w':
		snake->dy = -1;
		snake->dx = 0;
		break;
	case's': 
		snake->dy = 1;
		snake->dx = 0;
		break;
	case'a':
		snake->dy = 0;
		snake->dx = -1;
		break;
	case'd':
		snake->dy = 0;
		snake->dx = 1;
		break;	
	}
}

void snake_eat_food(SNAKE* snake)
{
	//判断是否吃到食物
	if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	{
		//吃到了食物，身体加强，食物消失，重新生成新的食物，
		snake->size++;//这里不需要存储食物的坐标。//这里不需要把食物消失，只要重新生成就行
		init_food(&(snake->food));
		snake->score += 10;
	}
}

void game_over(SNAKE* snake)
{
	//清除蛇尾痕迹
	snake->coord.X = 40;
	snake->coord.Y = 25;
	//定位光标的位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("游戏结束，总分数:%d\n", snake->score);
}

void snake_eat_body(SNAKE* snake)
{
	for(int i = 1;i< snake->size;i++)
	{
		if(snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y)
		{
			//吃到身体；游戏结束
			game_over(snake);
			exit(0);

		}
	}
}



void start_game(SNAKE* snake)
{
	while(snake->list[0].x >=0 && snake->list[0].x < W && snake->list[0].y > 0 && snake->list[0].y < H) 
	{
		//显示蛇和食物
		show_ui(snake);
		
		//控制移动方向
		contorl_snake(snake);
		 
		//判断是否吃食物
		snake_eat_food(snake);

		//判断是否吃到身体
		snake_eat_body(snake);

		// 移动蛇
		move_snake(snake);

		//延迟0.3s
		Sleep(200);

	}
	//碰到墙壁游戏结束
	game_over(snake);
}

int main(int argc,char* argv[])
{
	//隐藏光标
	hide_cur();
	SNAKE* snake = (SNAKE *)malloc(sizeof(SNAKE));
	//初始化蛇和食物
	init_snake(snake);
	//显示地图边界
	show_wall();
	//显示蛇和食物
	show_ui(snake);
	//启动游戏
	start_game(snake);





	free(snake);
	//while (1);

	return 0;
}