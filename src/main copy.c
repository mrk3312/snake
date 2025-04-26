void MoveSnake(SnakeData *snakeData, Cell map[20][20])
{
	switch (snakeData->dir)
	{
	case LEFT:
	{
		if (snakeData->head.x != 0)
		{
			map[snakeData->tail.x][snakeData->tail.y].renderTexture = NONE;
			if (snakeData->len == 2)
			{
				snakeData->tail.x = snakeData->head.x;
				snakeData->tail.y = snakeData->head.y;
			}
			snakeData->head.x--;
			map[snakeData->tail.x][snakeData->tail.y].renderTexture = TAIL;
			map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
		}
		break;
	}
	case RIGHT:
	{
		if (snakeData->head.x != 19)
		{	
			map[snakeData->tail.x][snakeData->tail.y].renderTexture = NONE;
			if (snakeData->len == 2)
			{
				snakeData->tail.x = snakeData->head.x;
				snakeData->tail.y = snakeData->head.y;
			}
			snakeData->head.x++;
			map[snakeData->tail.x][snakeData->tail.y].renderTexture = TAIL;
			map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
		}
		break;
	}
	case UP:
	{
		if (snakeData->head.y != 0)
		{
			if (snakeData->len == 2)
			{
				map[snakeData->tail.x][snakeData->tail.y].renderTexture = NONE;
				if (snakeData->len == 2)
				{
					snakeData->tail.x = snakeData->head.x;
					snakeData->tail.y = snakeData->head.y;
				}
				snakeData->head.y--;
				map[snakeData->tail.x][snakeData->tail.y].renderTexture = TAIL;
				map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
			}
		}
		break;
	}
	case DOWN:
	{
		if (snakeData->head.y != 19)
		{
			if (snakeData->len == 2)
			{
				map[snakeData->tail.x][snakeData->tail.y].renderTexture = NONE;
				if (snakeData->len == 2)
				{
					snakeData->tail.x = snakeData->head.x;
					snakeData->tail.y = snakeData->head.y;
				}
				snakeData->head.y++;
				map[snakeData->tail.x][snakeData->tail.y].renderTexture = TAIL;
				map[snakeData->head.x][snakeData->head.y].renderTexture = HEAD;
			}
		}
		break;
	}
	case NOTCHANGED: break;
	}
	if (map[snakeData->head.x][snakeData->head.y].containsFruit)
	{
		map[snakeData->head.x][snakeData->head.y].containsFruit = false;
		snakeData->len++;
	}
}