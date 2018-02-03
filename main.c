#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <inttypes.h>
#include <stdbool.h>
#include <iso646.h>

#define SIZE 4

typedef struct
{
	int32_t array[SIZE][SIZE];
	int32_t score;
	int32_t highest_score;
	int numbers;
}home;

void showLine(int);
void showMiddleLine(int);
void showResult(home*);
void reset(home*);
void move(home*, int*, int*, int*, int*);
void moveToUp(home*);
void moveToDown(home*);
void moveToLeft(home*);
void moveToRight(home*);
void receiveOperation(home*);
int twoOrFour(void);
void setValue(home*, int position, int value);
void creatNewNumber(home*);
bool isEqual(home*, home*);
bool isOver(home*);
int32_t readHighestScore();
void setHighestScore(home*);

int main(void)
{
	home h;
	home temp_h = h;
	bool is_over = false;

	reset(&h);
	for (;;)
	{
		temp_h = h;
		showResult(&h);
		receiveOperation(&h);
		showResult(&h);
		if (not(isEqual(&temp_h, &h)))
		{
			creatNewNumber(&h);
			showResult(&h);
			is_over = isOver(&h);
			if (is_over)
				break;
		}
	}
	printf("Game Over!\n");
    return 0;
}

void showResult(home* h)
{
	system("cls");
	printf("Score: %d", h->score);
	printf("     Best: %d\n", h->highest_score);
	showLine(15);
	for (int i=0; i<SIZE; i++)
	{
		for (int j=0; j<SIZE; j++)
		{
			printf("|");
			if (h->array[i][j] != 0)
			    printf("%6d", h->array[i][j]);
			else
				printf("      ");
		}
		printf("|\n");
		if (i != (SIZE - 1))
			showMiddleLine(3);
	}
	showLine(15);
}

void showLine(int length)
{
	for (int i=0; i<length; i++)
	{
		printf("- ");
	}
	printf("\n");
}

void reset(home* h)
{
	FILE* fp;

	if ((fp = fopen("highest_score", "r")) == NULL)
	{
		h->highest_score = 0;
		fp = fopen("highest_score", "w");
		fprintf(fp, "%d", 0);
	}
	else
	{
		h->highest_score = readHighestScore();
	}
	fclose(fp);
	srand((unsigned int)time(0));
	h->numbers = 2;
	h->score = 0;
	for (int i=0; i<SIZE; i++)
		for (int j=0; j<SIZE; j++)
			h->array[i][j] = 0;

	setValue(h, rand() % 16, twoOrFour());
	setValue(h, rand() % 15, twoOrFour());
}

void showMiddleLine(int m_length)
{
	for (int i=0; i<SIZE; i++)
	{
		printf("|");
		for (int j=0; j<m_length; j++)
		{
			printf("- ");
		}
	}
	printf("|\n");
}

void move(home* h, int* n1, int* n2, int* n3, int* n4)
/*
 * 从n1向n4移动
*/
{
	if (*n2 == 0)
	{
		*n2 = *n1;
		*n1 = 0;
	}
	if (*n3 == 0)
	{
 		*n3 = *n2;
		*n2 = *n1;
		*n1 = 0;
	}
	if (*n4 == 0)
	{
		*n4 = *n3;
 		*n3 = *n2;
		*n2 = *n1;
		*n1 = 0;
	}
	if (*n4 == *n3)
	{
		*n4 = *n4 + *n3;
		h->score += *n4;
		setHighestScore(h);
		if (*n4 != 0)
		    h->numbers--;
		if (*n2 == *n1)
		{
			*n3 = *n2 + *n1;
			h->score += *n3;
			setHighestScore(h);
			if (*n3 != 0)
			    h->numbers--;
			*n2 = 0;
			*n1 = 0;
		}
		else
		{
			*n3 = *n2;
			*n2 = *n1;
			*n1 = 0;
		}
	}
	else
	{
		if (*n3 == *n2)
		{
			*n3 = *n2 + *n3;
			*n2 = *n1;
			*n1 = 0;
			h->score += *n3;
			setHighestScore(h);
			if (*n3 != 0)
				h->numbers--;
		}
		else if (*n2 == *n1)
		{
			*n2 = *n2 + *n1;
			*n1 = 0;
			h->score += *n2;
			setHighestScore(h);
			if (*n2 != 0)
			    h->numbers--;
		}
	}
}

void moveToUp(home* h)
{
	for (int i=0; i<=3; i++)
		move(h, &(h->array[3][i]), &(h->array[2][i]), &(h->array[1][i]), &(h->array[0][i]));
}

void moveToDown(home* h)
{
	for (int i=0; i<=3; i++)
		move(h, &(h->array[0][i]), &(h->array[1][i]), &(h->array[2][i]), &(h->array[3][i]));
}

void moveToLeft(home* h)
{
	for (int i=0; i<=3; i++)
		move(h, &(h->array[i][3]), &(h->array[i][2]), &(h->array[i][1]), &(h->array[i][0]));
}

void moveToRight(home* h)
{
	for (int i=0; i<=3; i++)
		move(h, &(h->array[i][0]), &(h->array[i][1]), &(h->array[i][2]), &(h->array[i][3]));
}

void receiveOperation(home* h)
{
	int c = getch();

	if (c == 'w')
		moveToUp(h);
	else if (c == 's')
		moveToDown(h);
	else if (c == 'a')
		moveToLeft(h);
	else if (c == 'd')
		moveToRight(h);
}

int twoOrFour()
{
	srand((unsigned int)time(0));
	if (rand()%10 == 0)    //10%的概率出现4
		return 4;
	else
		return 2;
}

void setValue(home* h, int remainPosition, int value)    //remainPosition 为产生的数字为0的某个数位置
{
	int count = 0;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (h->array[i][j] == 0 && count == remainPosition)
            {
                h->array[i][j] = value;
                return;
            }
            else if (h->array[i][j] == 0 && count != remainPosition)
                count++;
        }
}

void creatNewNumber(home* h)
{
	int remain = 16 - h->numbers;

	setValue(h, rand()%remain+1, twoOrFour());
	h->numbers++;
}

bool isEqual(home* h1, home* h2)
{
	for (int i=0; i<SIZE; i++)
		for (int j=0; j<SIZE; j++)
		{
			if (h1->array[i][j] != h2->array[i][j])
				return false;
		}
	return true;
}

bool isOver(home* h)
{
	home temp_h = *h;

	if (h->numbers == 16)
	{
		moveToUp(&temp_h);
		if (isEqual(&temp_h, h))
		{
			moveToDown(&temp_h);
			if (isEqual(&temp_h, h))
			{
				moveToLeft(&temp_h);
				if (isEqual(&temp_h, h))
				{
					moveToRight(&temp_h);
					if (isEqual(&temp_h, h))
						return true;
				}
			}
		}
	}
	return false;
}

int32_t readHighestScore()
{
	FILE* fp;
	int32_t score;

	if ((fp = fopen("highest_score", "r")) == NULL)
	{
		printf("This file does not exist\n");
		exit(-1);
	}
	fscanf(fp, "%d", &score);
	fclose(fp);
	return score;
}

void setHighestScore(home* h)
{
	if (h->score > h->highest_score)
	{
		FILE* fp;

		if ((fp = fopen("highest_score", "w")) == NULL)
		{
			printf("Failed\n");
			exit(-1);
		}
		fprintf(fp, "%d", h->score);
		h->highest_score = h->score;
	}
}
