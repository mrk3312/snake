#include "snake.h"

void InitGame(Cell map[20][20], SnakeData *snakeData)
{

	InitWindow(1000, 1000, "Snake");
	snakeData->snakeDirData.futureDir = NOTCHANGED;
	snakeData->snakeDirData.currentDir = NOTCHANGED;
	snakeData->head.prevDir = NOTCHANGED;
	GenerateMap(map);
	InitSnake(map, snakeData);
}

void GenerateMap(Cell map[20][20])
{
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			map[x][y] = (Cell){.pos.x = x, .pos.y = y, .cellDirData.lastDir = NOTCHANGED, .cellDirData.currentDir = NOTCHANGED, .cellDirData.futureDir = NOTCHANGED, .isBorder = (x == 0 || y == 0 || x == 19 || y == 19) ? true : false, .renderTexture = NONE};
		}
	}
}

void GenerateFruit(Cell map[20][20], GameState *gameState)
{
	while(!gameState->isFruitPlaced)
    {
        Vec2 fruitPos = (Vec2){.x = 1 + (rand() % 17), .y = 1 + (rand() % 17)};
        if (map[fruitPos.x][fruitPos.y].renderTexture == NONE)
        {
            gameState->hasHeadEatenFruit = false;
            map[fruitPos.x][fruitPos.y].renderTexture = FRUIT;
            gameState->isFruitPlaced = true;
			printf("Tried to generate Fruit\n");
        }
    }
}
void RenderMap(Cell map[20][20], SnakeData *snakeData, GameState *gameState, Texture2D *spriteSheet)
{
    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
			Cell *cell = &map[x][y];

			switch(cell->renderTexture)
			{
				case HEAD:
				{
					switch(snakeData->snakeDirData.currentDir)
					{
						case LEFT:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){192, 64, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
						case NOTCHANGED:
						case RIGHT:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){256, 0, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
						case UP:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){192, 0, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
						case DOWN:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){256, 64, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
					}
					break;
				}
				case BODY:
				{
					// if((cell->cellDirData.lastDir == LEFT && cell->cellDirData.futureDir == LEFT))
					// 	DrawTexturePro(*spriteSheet, (Rectangle){64, 0, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
					switch(cell->cellDirData.futureDir)
					{
						case NOTCHANGED:
						case LEFT:
						case RIGHT:
						{
							switch (cell->cellDirData.lastDir)
							{
								case NOTCHANGED:
								case LEFT:
								case RIGHT:
								{
									DrawTexturePro(*spriteSheet, (Rectangle){64, 0, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
									break;
								}
								case UP:
								{
									DrawTexturePro(*spriteSheet, (Rectangle){128, 128, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
									break;
								}
								case DOWN:
								{
									DrawTexturePro(*spriteSheet, (Rectangle){0, 64, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
									break;
								}
							}
							break;
						}
						case UP:
						case DOWN:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){128, 64, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}		
					}
					break;
				}
				case TAIL:
				{
					switch(cell->cellDirData.futureDir)
					{
						case LEFT:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){192, 192, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
						case RIGHT:
						case NOTCHANGED:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){256, 128, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
						case UP:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){192, 128, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
						case DOWN:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){256, 192, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
							break;
						}
					}
					break;
				}
				case FRUIT:
				{
					DrawTexturePro(*spriteSheet, (Rectangle){0, 192, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
					break;
				}
			}

			// OLD RENDERING
            // if (map[x][y].renderTexture == HEAD)
            // 	DrawRectangle(x * 50, y * 50, 50, 50, RED);
            // else if (map[x][y].renderTexture == TAIL)
            //     DrawRectangle(x * 50, y * 50, 50, 50, BLUE);
            // else if (map[x][y].renderTexture == BODY)
            //     DrawRectangle(x * 50, y * 50, 50, 50, DARKGRAY);
			// else if (map[x][y].renderTexture == FRUIT)
            //     DrawRectangle(x * 50, y * 50, 50, 50, GREEN);
            
            // if (map[x][y].containsFruit)
			// 	DrawTexturePro(*spriteSheet, (Rectangle){0, 192, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
                //DrawRectangle(x * 50, y * 50, 50, 50, GREEN);
            if (map[x][y].isBorder)
            	DrawRectangle(x * 50, y * 50, 50, 50, DARKGREEN);
        }
    }
}

void InitSnake(Cell map[20][20], SnakeData *snakeData)
{
	map[3][9].renderTexture = TAIL;
	map[4][9].renderTexture = BODY;
	map[5][9].renderTexture = HEAD;
	snakeData->head.pos.x = map[5][9].pos.x;
	snakeData->head.pos.y = map[5][9].pos.y;
}

void InitGameState(GameState *gameState)
{
	gameState->initMovement = false;
	gameState->isHeadPosUpdated = false;
	gameState->hasHeadEatenFruit = false;
	gameState->isFruitPlaced = false;
}

void RefreshGame(Cell map[20][20], SnakeData *snakeData, GameState *gameState)
{
	InitGameState(gameState);
	snakeData->snakeDirData.futureDir = NOTCHANGED;
	snakeData->snakeDirData.currentDir = NOTCHANGED;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			cell->renderTexture = NONE;
			cell->cellDirData.futureDir = NOTCHANGED;
			cell->cellDirData.currentDir = NOTCHANGED;
		}
	}
	InitSnake(map, snakeData);
	GenerateFruit(map, gameState);
}


