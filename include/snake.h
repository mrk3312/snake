#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum GameTexture
{
	NONE,
	HEAD,
	BODY,
	TAIL,
	FRUIT
} GameTexture;

typedef enum Direction
{
	NOTCHANGED,
	LEFT,
	RIGHT,
	UP,
	DOWN,
} Direction;

typedef struct Vec2
{
	int x,y;
}Vec2;

typedef struct DirectionData
{
	Direction currentDir;
	Direction futureDir;
} DirectionData;

typedef struct Cell
{
	Vec2 pos;
	GameTexture renderTexture;
	Direction cellFutureDir;
	bool isBorder, headChangedDirection;
} Cell;

typedef struct SnakeData
{
	Vec2 head;
	bool headChangedDirection;
} SnakeData;

typedef struct GameState
{
	bool isHeadPosUpdated, initMovement, hasHeadEatenFruit, isFruitPlaced;
	Direction currentDirection;
	Direction futureDirection;
} GameState;

typedef struct BodyData 
{
	Vec2 pos;
	GameTexture renderTexture;
} BodyData;

typedef struct DynamicArray
{
    int capacity;
    int numberBodyParts;
    BodyData *bodyData;
}DynamicArray;

Direction DirectionKey(void);
void UpdateHeadPos(SnakeData*, GameState*, Cell[20][20]);
void UpdateSnakeDir(SnakeData*, GameState*);
void ClearGraphics(Cell[20][20]);
void ManageArraySize(DynamicArray*);
void CopyCellDataToArray(DynamicArray*, Cell*);
void IncreaseSnakeSize(DynamicArray*, Cell*);
void UpdateBodyPos(DynamicArray*, Cell*);
void UpdateBody(Cell[20][20], DynamicArray*, GameState*);
void PutSnakeOnMap(Cell[20][20], DynamicArray*, SnakeData*, GameState*);

bool IsHorizontal(Direction dir);
bool IsVertical(Direction dir);
bool CanMovementBeUpdated(GameState*);
bool DoesHeadTouchBodyOrBorder(SnakeData*, Cell[20][20]);
bool HasHeadEaten(SnakeData*, Cell[20][20]);

void GenerateMap(Cell[20][20]);
void GenerateFruit(Cell[20][20], GameState*);
void RenderMap(Cell[20][20], SnakeData*, GameState*, Texture2D*);
void RefreshGame(Cell[20][20], SnakeData*, GameState*);
void InitGame(Cell[20][20], SnakeData*);

void InitSnake(Cell[20][20],SnakeData*);
void InitGameState(GameState*);

#endif
