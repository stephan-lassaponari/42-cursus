#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void print_array(char **arr, int width, int height)
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			putchar(arr[i][j]);
		}
		putchar('\n');
	}
}

char **create_array(int width, int height)
{
	char **arr = malloc(sizeof(char *) * height);
	for(int i = 0; i < height; i++)
	{
		arr[i] = malloc(width);
		for(int j = 0; j < width; j++)
		{
			arr[i][j] = ' ';
		}
	}
	return arr;
}

void free_array(char **arr, int height)
{
	for(int i = 0; i < height; i++)
		free(arr[i]);
	free(arr);
}

void draw_pattern(char **arr, int width, int height)
{
	int x = 0, y = 0;
	int drawing = 0;
	char c;

	while(read(0, &c, 1) > 0)
	{
		if (c == 'x')
			drawing = !drawing;
		else if (c == 'w' && y > 0)
			y--;
		else if (c == 's' && y < height - 1)
			y++;
		else if (c == 'a' && x > 0)
			x--;
		else if (c == 'd' && x < width - 1)
			x++;

		if (drawing && x >= 0 && x < width && y >= 0 && y < height)
			arr[y][x] = '0';
	}
}

int count_neighbors(char **arr, int width, int height, int y, int x)
{
	int count = 0;

	for(int dy = -1; dy <= 1; dy++)
	{
		for(int dx = -1; dx <= 1; dx++)
		{
			if (dy == 0 && dx == 0)
				continue;

			int ny = y + dy;
			int nx = x + dx;

			if (ny >= 0 && ny < height && nx >= 0 && nx < width)
			{
				if (arr[ny][nx] == '0')
					count++;
			}
		}
	}
	return count;
}

void game_of_life(char **arr, int width, int height)
{
	char **new_arr = create_array(width, height);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			int neighbors = count_neighbors(arr, width, height, i, j);

			if (arr[i][j] == '0')
			{
				if (neighbors == 2 || neighbors == 3)
					new_arr[i][j] = '0';
				else
					new_arr[i][j] = ' ';
			}
			else
			{
				if (neighbors == 3)
					new_arr[i][j] = '0';
				else
					new_arr[i][j] = ' ';
			}
		}
	}

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			arr[i][j] = new_arr[i][j];
		}
	}

	free_array(new_arr, height);
}

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;

	int width = atoi(av[1]);
	int height = atoi(av[2]);
	int iterations = atoi(av[3]);

	char **arr = create_array(width, height);

	draw_pattern(arr, width, height);

	for(int i = 0; i < iterations; i++)
	{
		game_of_life(arr, width, height);
	}

	print_array(arr, width, height);
	free_array(arr, height);

	return 0;
}
