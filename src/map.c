#include "snake.h"

void InitGame(Cell map[20][20], SnakeData *snakeData, SnakeState *snakeState)
{
	InitWindow(1000, 1000, "Snake");
	snakeData->dir = NOTCHANGED;
	GenerateMap(map);
	InitSnake(map, snakeData);
	InitSnakeState(snakeState);
}

void GenerateMap(Cell map[20][20])
{
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			map[x][y] = (Cell){.pos.x = x, .pos.y = y, .cellDir = NOTCHANGED, .isBorder = (x == 0 || y == 0 || x == 19 || y == 19) ? true : false, .containsFruit = false, .renderTexture = NONE};
		}
	}
}

void RenderMap(Cell map[20][20])
{
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

            if (map[x][y].isBorder)
                DrawRectangle(x * 50, y * 50, 50, 50, DARKGREEN);
        }
    }
}

void InitSnake(Cell map[20][20], SnakeData *snakeData)
{
	map[3][1].renderTexture = TAIL;
	map[4][1].renderTexture = HEAD;
	map[5][4].containsFruit = true;
    map[10][4].containsFruit = true;
	snakeData->head.x = map[4][1].pos.x;
	snakeData->head.y = map[4][1].pos.y;
}

void InitSnakeState(SnakeState *snakeState)
{
	snakeState->initMovement = false;
	snakeState->isHeadPosUpdated = false;
	snakeState->isPlayerDead = false;
}

void RefreshGame(Cell map[20][20], SnakeData *snakeData, SnakeState *snakeState)
{
	InitSnakeState(snakeState);
	snakeData->dir = NOTCHANGED;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			cell->renderTexture = NONE;
			cell->cellDir = NONE;
		}
	}
	InitSnake(map, snakeData);
}


