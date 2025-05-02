#include "snake.h"

int main(void)
{
	Cell map[20][20];
	SnakeState snakeState;
	SnakeData snakeData;

	InitGame(map, &snakeData, &snakeState);

	float skipTimer = 0.0f;
	while (!WindowShouldClose())
	{
		if (!snakeState.initMovement)
		{
			Direction futureDirection = DirectionKey();
			if (futureDirection != NOTCHANGED && futureDirection != LEFT)
			{
				snakeState.initMovement = true;
				map[3][1].cellDir = RIGHT;
			}
		}

		float deltaTime = GetFrameTime();
		Direction futureDirection = DirectionKey();

		if (CanMovementBeUpdated(futureDirection, &snakeData, &snakeState))
		{
			UpdateSnakeDir(&snakeData, &snakeState, futureDirection);
			snakeData.dir = futureDirection;
			snakeState.isHeadPosUpdated = false;
		}
		skipTimer -= deltaTime;

		if(skipTimer <= 0)
		{
			MoveSnake(&snakeData, map);
			snakeState.isHeadPosUpdated = true;
			snakeState.isPlayerDead = DoesHeadTouchBodyOrBorder(&snakeData, map);

			if (snakeState.isPlayerDead)
				RefreshGame(map, &snakeData, &snakeState);

			skipTimer += 0.15f;
		}
		ClearBackground(RAYWHITE);
		BeginDrawing();
		RenderMap(map);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}