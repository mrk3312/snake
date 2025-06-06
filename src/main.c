#include "snake.h"

int main(void)
{	
	srand(time(NULL));
	Cell map[20][20];
	GameState gameState;
	SnakeData snakeData;

	InitGame(map, &snakeData);
	InitGameState(&gameState);
	GenerateFruit(map, &gameState);

	Texture2D spriteSheet = LoadTexture("spritesheet.png");
	float skipTimer = 0.0f;

	while (!WindowShouldClose())
	{
		if (!gameState.initMovement)
		{
			gameState.futureDirection = DirectionKey();
			if (gameState.futureDirection != NOTCHANGED && gameState.futureDirection != LEFT)
			{
				gameState.initMovement = true;
				map[3][9].cellFutureDir = RIGHT;
				map[4][9].cellFutureDir = RIGHT;
			}
		}

		float deltaTime = GetFrameTime();
		gameState.futureDirection = DirectionKey();

		if (CanMovementBeUpdated(&gameState))
		{
			UpdateSnakeDir(&snakeData, &gameState);
		}
		
		skipTimer -= deltaTime;

		if(skipTimer <= 0)
		{
			UpdateHeadPos(&snakeData, &gameState, map);
			gameState.isHeadPosUpdated = true;

			if (DoesHeadTouchBodyOrBorder(&snakeData, map))
			{
				RefreshGame(map, &snakeData, &gameState);
				printf("snake died\n");
				//currentDirection = NONE; // Temporary solution (if it's in refresh game it doesn't work)
			}
			
			if (HasHeadEaten(&snakeData, map)) // Will head eat (based on the updated coords)
			{
				//map[snakeData.head.x][snakeData.head.y].renderTexture = NONE;
				gameState.hasHeadEatenFruit = true;
			}

			DynamicArray dynamicArray = (DynamicArray){.bodyData = NULL, .capacity = 0, .numberBodyParts = 0};
			UpdateBody(map, &dynamicArray, &gameState);
			PutSnakeOnMap(map, &dynamicArray, &snakeData, &gameState);
			free(dynamicArray.bodyData);
			skipTimer += 1.0f;
		}
		ClearBackground(RAYWHITE);
		BeginDrawing();
		RenderMap(map, &snakeData, &gameState, &spriteSheet);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}