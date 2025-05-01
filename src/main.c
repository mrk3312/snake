#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum Snake
{
	NONE,
	HEAD,
	BODY,
	TAIL
} Snake;

typedef enum Direction
{
	NOTCHANGED,
	LEFT,
	RIGHT,
	UP,
	DOWN
} Direction;

typedef struct Vec2
{
	int x,y;
}Vec2;

typedef struct Cell
{
	Vec2 pos;
	Snake renderTexture;
	Direction cellDir;
	bool containsFruit;
} Cell;

typedef struct BodyData 
{
	Vec2 pos;
	Snake renderTexture;
} BodyData;

typedef struct SnakeData
{
	Direction dir;
	Vec2 head;
} SnakeData;

bool IsHorizontal(Direction dir) {return dir == LEFT || dir == RIGHT;}
bool IsVertical(Direction dir) {return dir == UP || dir == DOWN;}

Direction DirectionKey(void)
{
	if (IsKeyPressed(KEY_A)) return LEFT;
	else if (IsKeyPressed(KEY_D)) return RIGHT;
	else if (IsKeyPressed(KEY_W)) return UP;
	else if (IsKeyPressed(KEY_S)) return DOWN;
	else return NOTCHANGED;
}

bool IsHeadPosInSync(SnakeData *snakeData)
{
	return false;
}
void MoveSnake(SnakeData *snakeData, Cell map[20][20])
{
	bool hasPlayerEatenFruit = false;
	bool hasPlayerGrown = false;

	// Part of the function that moves the head
	switch (snakeData->dir)
	{
	case LEFT:
	{
		map[snakeData->head.x][snakeData->head.y].cellDir = LEFT;
		map[snakeData->head.x][snakeData->head.y].renderTexture = NONE;
		snakeData->head.x--;
		map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
		break;
	}
	case RIGHT:
	{
		map[snakeData->head.x][snakeData->head.y].cellDir = RIGHT;
		map[snakeData->head.x][snakeData->head.y].renderTexture = NONE;
		snakeData->head.x++;
		map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
		break;
	}
	case UP:
	{
		map[snakeData->head.x][snakeData->head.y].cellDir = UP;
		map[snakeData->head.x][snakeData->head.y].renderTexture = NONE;
		snakeData->head.y--;
		map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
		break;
	}
	case DOWN:
	{
		
		map[snakeData->head.x][snakeData->head.y].cellDir = DOWN;
		map[snakeData->head.x][snakeData->head.y].renderTexture = NONE;
		snakeData->head.y++;
		map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
		break;
	}
	case NOTCHANGED: break;
	}
	if (map[snakeData->head.x][snakeData->head.y].containsFruit)
	{
		map[snakeData->head.x][snakeData->head.y].containsFruit = false;
		hasPlayerEatenFruit = true;
	}


	// Determine tail and body positions
	BodyData *bodyData = NULL;
	int capacity = 0;
	int numberBodyParts = 0;

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			if(cell->renderTexture == BODY || cell->renderTexture == TAIL)
			{
				if (capacity < numberBodyParts + 2)
				{
					capacity = (capacity == 0) ? 4 : capacity * 2;
					BodyData *temp = realloc(bodyData, sizeof(BodyData) * capacity);
					if (temp == NULL)
					{
						printf("Memory Allocation Error");
					}
					bodyData = temp;
				}
				bodyData[numberBodyParts].pos = cell->pos;
				bodyData[numberBodyParts].renderTexture = cell->renderTexture;
				
				if (hasPlayerEatenFruit && cell->renderTexture == TAIL)
					numberBodyParts++;

				switch(cell->cellDir)
				{
					case LEFT:
					{
						if (hasPlayerEatenFruit && cell->renderTexture == TAIL)
						{
							// add body data to next array element
							bodyData[numberBodyParts].pos = map[x-1][y].pos;
							hasPlayerGrown = true;
						}
						else
							// move the part to the left
							bodyData[numberBodyParts].pos.x--;
						break;
					}
					case RIGHT:
					{
						if (hasPlayerEatenFruit && cell->renderTexture == TAIL)
						{
							// add body data to next array element
							bodyData[numberBodyParts].pos = map[x+1][y].pos;
							hasPlayerGrown = true;
						}
						else
							// move the part to the right
							bodyData[numberBodyParts].pos.x++;
						break;
					}
					case UP:
					{
						if (hasPlayerEatenFruit && cell->renderTexture == TAIL)
						{
							// add body data to next array element
							bodyData[numberBodyParts].pos = map[x][y-1].pos;
							hasPlayerGrown = true;
						}
						else
							// move the part up
							bodyData[numberBodyParts].pos.y--;
						break;
					}
					case DOWN: 
					{
						if (hasPlayerEatenFruit && cell->renderTexture == TAIL)
						{
							// add body data to next array element
							bodyData[numberBodyParts].pos = map[x][y+1].pos;
							hasPlayerGrown = true;
						}
						else
							// move the part down
							bodyData[numberBodyParts].pos.y++;
						break;
					}
				}

				if (hasPlayerGrown)
				{
					bodyData[numberBodyParts].renderTexture = BODY;
					hasPlayerEatenFruit = false;
				}

				numberBodyParts++;
			}
		}
	}

	printf("number of body parts:%d\n", numberBodyParts);
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			if (cell->renderTexture != HEAD && cell->renderTexture != NONE)
			{
				cell->renderTexture = NONE;
			}
		}
	}

	for (int i = 0; i < numberBodyParts; i++)
	{
		Cell *cell = &map[bodyData[i].pos.x][bodyData[i].pos.y];
		cell->renderTexture = bodyData[i].renderTexture;
	}

	free(bodyData);
}

int main()
{
	InitWindow(1000, 1000, "Snake");
	Cell map[20][20];
	Direction currentDirection = NOTCHANGED;
	SnakeData snakeData;
	snakeData.dir = NOTCHANGED;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			map[x][y] = (Cell){.pos.x = x, .pos.y = y, .cellDir = NOTCHANGED, .containsFruit = false, .renderTexture = NONE};
		}
	}
	map[0][0].renderTexture = TAIL;
	map[1][0].renderTexture = HEAD;
	map[4][4].containsFruit = true;
	snakeData.head.x = map[1][0].pos.x;
	snakeData.head.y = map[1][0].pos.y;
	float skipTimer = 0.0f;

	bool isHeadPosUpdated = false;
	bool initMovement = false;
	while (!WindowShouldClose())
	{
		if (!initMovement)
		{
			Direction futureDirection = DirectionKey();
			if (futureDirection != NOTCHANGED)
			{
				initMovement = true;
				map[0][0].cellDir = RIGHT;
			}
		}
		float deltaTime = GetFrameTime();
		Direction futureDirection = DirectionKey();

		if (futureDirection != NOTCHANGED && !((IsHorizontal(snakeData.dir) && IsHorizontal(futureDirection)) || (IsVertical(snakeData.dir) && IsVertical(futureDirection))) && isHeadPosUpdated)
		{
			snakeData.dir = futureDirection;
			isHeadPosUpdated = false;
		}
		skipTimer -= deltaTime;

		if(skipTimer <= 0)
		{
			MoveSnake(&snakeData, map);
			isHeadPosUpdated = true;
			skipTimer += 0.15f;
		}
		ClearBackground(RAYWHITE);
		BeginDrawing();
		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				if (map[x][y].renderTexture == HEAD)
				{
					DrawRectangle(x * 50, y * 50, 50, 50, RED);
				}
				else if (map[x][y].renderTexture == TAIL)
					DrawRectangle(x * 50, y * 50, 50, 50, BLUE);
				else if (map[x][y].renderTexture == BODY)
					DrawRectangle(x * 50, y * 50, 50, 50, DARKGRAY);
				
				if (map[x][y].containsFruit)
					DrawRectangle(x * 50, y * 50, 50, 50, GREEN);
			}
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}