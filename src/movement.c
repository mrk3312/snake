#include "snake.h"

bool IsHorizontal(Direction dir) {return dir == LEFT || dir == RIGHT;}
bool IsVertical(Direction dir) {return dir == UP || dir == DOWN;}
bool CanMovementBeUpdated(SnakeData *snakeData, GameState *gameState) {return snakeData->snakeDirData.futureDir != NOTCHANGED && !((IsHorizontal(snakeData->snakeDirData.currentDir) && IsHorizontal(snakeData->snakeDirData.futureDir)) || (IsVertical(snakeData->snakeDirData.currentDir) && IsVertical(snakeData->snakeDirData.futureDir))) && gameState->isHeadPosUpdated && gameState->initMovement;}
bool DoesHeadTouchBodyOrBorder(SnakeData *snakeData, Cell map[20][20]) {
	return map[snakeData->head.pos.x][snakeData->head.pos.y].isBorder || 
	        (map[snakeData->head.pos.x][snakeData->head.pos.y].renderTexture == TAIL || 
		    (map[snakeData->head.pos.x][snakeData->head.pos.y].renderTexture == BODY));
}
bool HasHeadEaten(SnakeData *snakeData, Cell map[20][20]) {return map[snakeData->head.pos.x][snakeData->head.pos.y].renderTexture == FRUIT;}

Direction DirectionKey(void)
{
	if (IsKeyPressed(KEY_A)) return LEFT;
	else if (IsKeyPressed(KEY_D)) return RIGHT;
	else if (IsKeyPressed(KEY_W)) return UP;
	else if (IsKeyPressed(KEY_S)) return DOWN;
	else return NOTCHANGED;
}

void UpdateSnakeDir(SnakeData *snakeData, GameState *gameState)
{
    snakeData->head.prevDir = snakeData->snakeDirData.currentDir;
    snakeData->snakeDirData.currentDir = snakeData->snakeDirData.futureDir;
    gameState->isHeadPosUpdated = false;
}

void UpdateHeadPos(SnakeData *snakeData, Cell map[20][20])
{
    map[snakeData->head.pos.x][snakeData->head.pos.y].cellDirData.futureDir = snakeData->snakeDirData.currentDir;
    map[snakeData->head.pos.x][snakeData->head.pos.y].cellDirData.lastDir = snakeData->head.prevDir;
    switch (snakeData->snakeDirData.currentDir)
    {
        case LEFT: snakeData->head.pos.x--; break;
        case RIGHT: snakeData->head.pos.x++; break;
        case UP: snakeData->head.pos.y--; break;
        case DOWN: snakeData->head.pos.y++; break;
        case NOTCHANGED: break;
    }
}

void ClearGraphics(Cell map[20][20])
{
    for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
            if (cell->renderTexture != FRUIT)
			    cell->renderTexture = NONE;
		}
	}
}

void ManageArraySize(DynamicArray *dynamicArray)
{
    if (dynamicArray->capacity < dynamicArray->numberBodyParts + 2)
    {
        dynamicArray->capacity = (dynamicArray->capacity == 0) ? 4 : dynamicArray->capacity * 2;
        BodyData *temp = realloc(dynamicArray->bodyData, sizeof(BodyData) * dynamicArray->capacity);
        if (temp == NULL)
        {
            printf("Memory Allocation Error");
        }
        dynamicArray->bodyData = temp;
    }
}

void CopyCellDataToArray(DynamicArray *dynamicArray, Cell *cell)
{
    dynamicArray->bodyData[dynamicArray->numberBodyParts].pos = cell->pos;
	dynamicArray->bodyData[dynamicArray->numberBodyParts].renderTexture = cell->renderTexture;
}

void IncreaseSnakeSize(DynamicArray *dynamicArray, Cell *cell)
{
    dynamicArray->numberBodyParts++;
    dynamicArray->bodyData[dynamicArray->numberBodyParts].pos = cell->pos;
    dynamicArray->bodyData[dynamicArray->numberBodyParts].renderTexture = BODY;
}

void UpdateBodyPos(DynamicArray *dynamicArray, Cell *cell)
{
    switch(cell->cellDirData.futureDir)
    {
        case LEFT: dynamicArray->bodyData[dynamicArray->numberBodyParts].pos.x--; break;
        case RIGHT: dynamicArray->bodyData[dynamicArray->numberBodyParts].pos.x++; break;
        case UP: dynamicArray->bodyData[dynamicArray->numberBodyParts].pos.y--; break;
        case DOWN: dynamicArray->bodyData[dynamicArray->numberBodyParts].pos.y++; break;
    }

    dynamicArray->numberBodyParts++;
}

void UpdateBody(Cell map[20][20], DynamicArray *dynamicArray, GameState *gameState)
{
    for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			Cell *cell = &map[x][y];
			if(cell->renderTexture == BODY || cell->renderTexture == TAIL)
			{

                ManageArraySize(dynamicArray);
				CopyCellDataToArray(dynamicArray, cell);
                
				if (gameState->hasHeadEatenFruit && cell->renderTexture == TAIL)
                {
					IncreaseSnakeSize(dynamicArray, cell);
                    gameState->isFruitPlaced = false;
                }
                UpdateBodyPos(dynamicArray, cell);
			}
		}
	}
}

void PutSnakeOnMap(Cell map[20][20], DynamicArray *dynamicArray, SnakeData *snakeData, GameState *gameState)
{
	ClearGraphics(map);
	map[snakeData->head.pos.x][snakeData->head.pos.y].renderTexture = HEAD;
    for (int i = 0; i < dynamicArray->numberBodyParts; i++)
	{
		Cell *cell = &map[dynamicArray->bodyData[i].pos.x][dynamicArray->bodyData[i].pos.y];
		cell->renderTexture = dynamicArray->bodyData[i].renderTexture;
	}
    GenerateFruit(map, gameState);
}

