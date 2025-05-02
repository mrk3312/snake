#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
	bool isBorder;
} Cell;

typedef struct SnakeState
{
	bool isHeadPosUpdated, initMovement, isPlayerDead;
} SnakeState;

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

Direction DirectionKey(void);
void MoveSnake(SnakeData*, Cell[20][20]);
void UpdateHeadPos(SnakeData*, Cell[20][20]);
void UpdateSnakeDir(SnakeData*, SnakeState*, Direction);

bool IsHorizontal(Direction dir);
bool IsVertical(Direction dir);
bool CanMovementBeUpdated(Direction, SnakeData*, SnakeState*);
bool DoesHeadTouchBodyOrBorder(SnakeData*, Cell[20][20]);

void GenerateMap(Cell[20][20]);
void RenderMap(Cell[20][20]);
void RefreshGame(Cell[20][20], SnakeData*, SnakeState*);
void InitGame(Cell[20][20], SnakeData*, SnakeState*);

void InitSnake(Cell[20][20],SnakeData*);
void InitSnakeState(SnakeState*);

#endif 
