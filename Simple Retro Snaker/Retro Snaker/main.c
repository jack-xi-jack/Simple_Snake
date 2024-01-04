#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>

//1. ���õ�ͼ�߽�
#define W 60
#define H 20

//2.����̰���߽ṹ��
typedef struct _body
{
	int x;  //����x
	int y;  //����y
}BODY;

typedef struct snake
{
	BODY list[W * H];  //̰�������ռ��������ͼ
	int size;          // ʵ���������;��ͷ(@)��������β(*)
	BODY food;         // ʳ�#
	COORD coord;		//����λ��꣩����	Window.hͷ�ļ��ж����ˣ�
	int dx;				//�ƶ�����	��1 ��-1		
	int dy;				//�ƶ�����	��1 ��-1
	BODY tail;          //��¼��β�����������β�ۼ���
	int score;			//��Ϸ������

}SNAKE;

//3.ʵ����ʾ��ͼ��������ʼ���ߵĺ�������ʼ��ʳ�ﺯ��


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
	// �������������
	srand(time(NULL));
	// ����ʳ������
	food->x = rand() % (W-1) +1;//ʳ�������x��60����
	food->y = rand() % (H-1) +1;//ʳ�������y��20����
}

void init_snake(SNAKE* snake)
{
	//������ͷ����βλ��
	snake->list[0].x = W / 2;
	snake->list[0].y = H / 2;
	snake->list[1].x = W / 2 - 1;
	snake->list[1].x = H / 2 - 1;
	snake->size = 2;

	//�����ƶ��ķ���
	snake->dx = 1;	//Ĭ�������ƶ�����������
	snake->dy = 0;

	snake->score = 0; //Ĭ�Ϸ�����0��

	// ����ʳ��λ��
	init_food(&(snake->food));

}

void show_ui(SNAKE* snake)
{
	//��ʾ��
	for(int i = 0; i < snake->size;i++)
	{
		snake->coord.X = snake->list[i]. x;
		snake->coord.Y = snake->list[i]. y;
		//��λ����λ��
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
		if (i == 0) //��ʾ��ͷ@
		{
			printf("@");
		}
		else     //��ʾ�������β
		{
			printf("*");
		}
		
	}

	//��ʾʳ��
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	//��λ����λ��
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("#");

	//�����β�ۼ�
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	//��λ����λ��
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

void hide_cur() //���ع��
{
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void move_snake(SNAKE * snake)
{
	//��¼��β����������ۼ���
	snake->tail = snake->list[snake->size - 1]; 
	//�ߵ��ƶ��ǴӺ��濪ʼ�ƶ�����ͷҪ��������Ҫ�ƶ�����
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
	while (_kbhit()) //�ж��Ƿ��°���,���²�����0 //��⵽���°����¼�
	{
		key = _getch();//��ȡ���µļ�ֵ��
	}
	//̧���¶��� ���� û�а��¶���

	//���ݰ����޸��ƶ�����
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
	//�ж��Ƿ�Ե�ʳ��
	if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	{
		//�Ե���ʳ������ǿ��ʳ����ʧ�����������µ�ʳ�
		snake->size++;//���ﲻ��Ҫ�洢ʳ������ꡣ//���ﲻ��Ҫ��ʳ����ʧ��ֻҪ�������ɾ���
		init_food(&(snake->food));
		snake->score += 10;
	}
}

void game_over(SNAKE* snake)
{
	//�����β�ۼ�
	snake->coord.X = 40;
	snake->coord.Y = 25;
	//��λ����λ��
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("��Ϸ�������ܷ���:%d\n", snake->score);
}

void snake_eat_body(SNAKE* snake)
{
	for(int i = 1;i< snake->size;i++)
	{
		if(snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y)
		{
			//�Ե����壻��Ϸ����
			game_over(snake);
			exit(0);

		}
	}
}



void start_game(SNAKE* snake)
{
	while(snake->list[0].x >=0 && snake->list[0].x < W && snake->list[0].y > 0 && snake->list[0].y < H) 
	{
		//��ʾ�ߺ�ʳ��
		show_ui(snake);
		
		//�����ƶ�����
		contorl_snake(snake);
		 
		//�ж��Ƿ��ʳ��
		snake_eat_food(snake);

		//�ж��Ƿ�Ե�����
		snake_eat_body(snake);

		// �ƶ���
		move_snake(snake);

		//�ӳ�0.3s
		Sleep(200);

	}
	//����ǽ����Ϸ����
	game_over(snake);
}

int main(int argc,char* argv[])
{
	//���ع��
	hide_cur();
	SNAKE* snake = (SNAKE *)malloc(sizeof(SNAKE));
	//��ʼ���ߺ�ʳ��
	init_snake(snake);
	//��ʾ��ͼ�߽�
	show_wall();
	//��ʾ�ߺ�ʳ��
	show_ui(snake);
	//������Ϸ
	start_game(snake);





	free(snake);
	//while (1);

	return 0;
}