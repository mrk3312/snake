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
	bool containsFruit;
	Snake renderTexture;
	Direction cellDir;
} Cell;

typedef struct SnakeData
{
	Direction dir;
	Vec2 head;
	int len;
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

void MoveHead(SnakeData *snakeData, Cell map[20][20])
{
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
		snakeData->len++;
	}
}

void MoveBody(Cell map[20][20])
{
	bool updatedTail = false;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			if (cell->renderTexture != HEAD)
			{
				switch (cell->cellDir)
				{
					case LEFT:
					{
						switch(map[x][y].renderTexture)
						{
							case TAIL:
							{
								if (!updatedTail)
								{
									map[x][y].renderTexture = NONE;
									map[x-1][y].renderTexture = TAIL;
									updatedTail = true;
								}
								break;
							}
						}
					}
					case RIGHT:
					{
						switch(map[x][y].renderTexture)
						{
							case TAIL:
							{
								if (!updatedTail)
								{
									map[x][y].renderTexture = NONE;
									map[x+1][y].renderTexture = TAIL;
									updatedTail = true;
								}
								break;
							}
						}
					}
					case UP:
					{
						switch(map[x][y].renderTexture)
						{
							case TAIL:
							{
								if (!updatedTail)
								{
									map[x][y].renderTexture = NONE;
									map[x][y-1].renderTexture = TAIL;
									updatedTail = true;
								}
								break;
							}
						}
					}
					case DOWN:
					{
						switch(map[x][y].renderTexture)
						{
							case TAIL:
							{
								if (!updatedTail)
								{
									map[x][y].renderTexture = NONE;
									map[x][y+1].renderTexture = TAIL;
									updatedTail = true;
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

int main()
{
	InitWindow(1000, 1000, "Snake");
	Cell map[20][20];
	Direction currentDirection = NOTCHANGED;
	SnakeData snakeData;
	snakeData.dir = NOTCHANGED;
	snakeData.len = 2;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			map[x][y] = (Cell){.pos.x = x, .pos.y = y, .cellDir = NOTCHANGED, .containsFruit = false, .renderTexture = NONE};
		}
	}
	map[0][0].renderTexture = TAIL;
	map[1][0].renderTexture = HEAD;
	map[15][15].containsFruit = true;
	snakeData.head.x = map[1][0].pos.x;
	snakeData.head.y = map[1][0].pos.y;

	float skipTimer = 0.0f;

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

		if (futureDirection != NOTCHANGED)
		{
			if (!((IsHorizontal(snakeData.dir) && IsHorizontal(futureDirection)) || (IsVertical(snakeData.dir) && IsVertical(futureDirection))))
				snakeData.dir = futureDirection;
		}
		skipTimer -= deltaTime;

		if(skipTimer <= 0)
		{
			MoveHead(&snakeData, map);
			MoveBody(map);
			skipTimer += 0.1f;
		}
		ClearBackground(RAYWHITE);
		BeginDrawing();
		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				if (map[x][y].renderTexture == HEAD)
					DrawRectangle(x * 50, y * 50, 50, 50, RED);
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