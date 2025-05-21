#include "snake.h"

bool IsHorizontal(Direction dir) {return dir == LEFT || dir == RIGHT;}
bool IsVertical(Direction dir) {return dir == UP || dir == DOWN;}
bool CanMovementBeUpdated(GameState *gameState) {return gameState->futureDirection != NOTCHANGED && !((IsHorizontal(gameState->currentDirection) && IsHorizontal(gameState->futureDirection)) || (IsVertical(gameState->currentDirection) && IsVertical(gameState->futureDirection))) && gameState->isHeadPosUpdated && gameState->initMovement;}
bool DoesHeadTouchBodyOrBorder(SnakeData *snakeData, Cell map[20][20]) {
	return map[snakeData->head.x][snakeData->head.y].isBorder || 
	        (map[snakeData->head.x][snakeData->head.y].renderTexture == TAIL || 
		    (map[snakeData->head.x][snakeData->head.y].renderTexture == BODY));
}
bool HasHeadEaten(SnakeData *snakeData, Cell map[20][20]) {return map[snakeData->head.x][snakeData->head.y].renderTexture == FRUIT;}

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
    gameState->currentDirection = gameState->futureDirection;
    snakeData->headChangedDirection = true;
    gameState->isHeadPosUpdated = false;
}

void UpdateHeadPos(SnakeData *snakeData, GameState *gameState, Cell map[20][20])
{
    map[snakeData->head.x][snakeData->head.y].cellFutureDir = gameState->currentDirection;
    if (snakeData->headChangedDirection)
        map[snakeData->head.x][snakeData->head.y].headChangedDirection = true;
    
    switch (gameState->currentDirection)
    {
        case LEFT: snakeData->head.x--; break;
        case RIGHT: snakeData->head.x++; break;
        case UP: snakeData->head.y--; break;
        case DOWN: snakeData->head.y++; break;
        case NOTCHANGED: break;
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
    switch(cell->cellFutureDir)
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
	map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
    for (int i = 0; i < dynamicArray->numberBodyParts; i++)
	{
		Cell *cell = &map[dynamicArray->bodyData[i].pos.x][dynamicArray->bodyData[i].pos.y];
		cell->renderTexture = dynamicArray->bodyData[i].renderTexture;
	}
    GenerateFruit(map, gameState);
}

