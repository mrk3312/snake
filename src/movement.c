#include "snake.h"

bool IsHorizontal(Direction dir) {return dir == LEFT || dir == RIGHT;}
bool IsVertical(Direction dir) {return dir == UP || dir == DOWN;}
bool CanMovementBeUpdated(Direction futureDirection, SnakeData *snakeData, SnakeState *snakeState) {return futureDirection != NOTCHANGED && !((IsHorizontal(snakeData->dir) && IsHorizontal(futureDirection)) || (IsVertical(snakeData->dir) && IsVertical(futureDirection))) && snakeState->isHeadPosUpdated && snakeState->initMovement;}
bool DoesHeadTouchBodyOrBorder(SnakeData *snakeData, Cell map[20][20]) {return map[snakeData->head.x][snakeData->head.y].isBorder || (map[snakeData->head.x][snakeData->head.y].renderTexture != NONE && (map[snakeData->head.x][snakeData->head.y].renderTexture != HEAD));}
bool HasHeadEaten(SnakeData *snakeData, Cell map[20][20]) {return map[snakeData->head.x][snakeData->head.y].containsFruit;}

Direction DirectionKey(void)
{
	if (IsKeyPressed(KEY_A)) return LEFT;
	else if (IsKeyPressed(KEY_D)) return RIGHT;
	else if (IsKeyPressed(KEY_W)) return UP;
	else if (IsKeyPressed(KEY_S)) return DOWN;
	else return NOTCHANGED;
}

void UpdateSnakeDir(SnakeData *snakeData, SnakeState *snakeState, Direction futureDirection)
{
    snakeData->dir = futureDirection;
    snakeState->isHeadPosUpdated = false; 
}

void UpdateHeadPos(SnakeData *snakeData, Cell map[20][20])
{

		map[snakeData->head.x][snakeData->head.y].cellDir = snakeData->dir;
		map[snakeData->head.x][snakeData->head.y].renderTexture = NONE;
		switch (snakeData->dir)
        {
            case LEFT: snakeData->head.x--; break;
            case RIGHT: snakeData->head.x++; break;
            case UP: snakeData->head.y--; break;
            case DOWN: snakeData->head.y++; break;
            case NOTCHANGED: break;
        }
		map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
}

void ClearBodyGraphics(Cell map[20][20])
{
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
}

void MoveSnake(SnakeData *snakeData, Cell map[20][20])
{
	bool hasPlayerEatenFruit = false;

	UpdateHeadPos(snakeData, map);

	if (HasHeadEaten(snakeData, map))
	{
		map[snakeData->head.x][snakeData->head.y].containsFruit = false;
		hasPlayerEatenFruit = true;
	}


	// Determine tail and body positions
	BodyData *bodyData = NULL;
	int capacity = 0;
	int numberBodyParts = 0;

    // should be function
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
                {
					numberBodyParts++;
                    bodyData[numberBodyParts].pos = cell->pos;
                    bodyData[numberBodyParts].renderTexture = BODY;
                    hasPlayerEatenFruit = false;
                }
                
				switch(cell->cellDir)
				{
					case LEFT: bodyData[numberBodyParts].pos.x--; break;
					case RIGHT: bodyData[numberBodyParts].pos.x++; break;
					case UP: bodyData[numberBodyParts].pos.y--; break;
					case DOWN: bodyData[numberBodyParts].pos.y++; break;
				}

				numberBodyParts++;
			}
		}
	}

	ClearBodyGraphics(map);

    //should be function
	for (int i = 0; i < numberBodyParts; i++)
	{
		Cell *cell = &map[bodyData[i].pos.x][bodyData[i].pos.y];
		cell->renderTexture = bodyData[i].renderTexture;
	}

	free(bodyData);
}

