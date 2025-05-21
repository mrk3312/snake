#include "snake.h"

void InitGame(Cell map[20][20], SnakeData *snakeData)
{

	InitWindow(1000, 1000, "Snake");
	GenerateMap(map);
	InitSnake(map, snakeData);
}

void GenerateMap(Cell map[20][20])
{
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			map[x][y] = (Cell){.pos.x = x, .pos.y = y, .cellFutureDir = NOTCHANGED, .headChangedDirection = false, .isBorder = (x == 0 || y == 0 || x == 19 || y == 19) ? true : false, .renderTexture = NONE};
		}
	}
}

void ClearGraphics(Cell map[20][20])
{
    for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			if (cell->renderTexture == NONE)
				cell->headChangedDirection = false;
            if (cell->renderTexture != FRUIT)
			    cell->renderTexture = NONE;
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
					switch(gameState->currentDirection)
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
					switch(cell->cellFutureDir)
					{
						case NOTCHANGED:
						case LEFT:
						case RIGHT:
						{
							DrawTexturePro(*spriteSheet, (Rectangle){64, 0, 64, 64},(Rectangle){x * 50, y * 50, 50, 50}, (Vector2) {0, 0}, 0.0f, WHITE);
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
					switch(cell->cellFutureDir)
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
	snakeData->head.x = map[5][9].pos.x;
	snakeData->head.y = map[5][9].pos.y;
}

void InitGameState(GameState *gameState)
{
	gameState->initMovement = false;
	gameState->isHeadPosUpdated = false;
	gameState->hasHeadEatenFruit = false;
	gameState->isFruitPlaced = false;
	gameState->futureDirection = NOTCHANGED;
	gameState->futureDirection = NOTCHANGED;
}

void RefreshGame(Cell map[20][20], SnakeData *snakeData, GameState *gameState)
{
	InitGameState(gameState);
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			cell->renderTexture = NONE;
			cell->cellFutureDir = NOTCHANGED;
			cell->headChangedDirection = false;
		}
	}
	InitSnake(map, snakeData);
	GenerateFruit(map, gameState);
}


