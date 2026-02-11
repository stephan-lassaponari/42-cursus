#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void print_array(char **arr, int w, int h)
{
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			putchar(arr[i][j]);
		}
		putchar('\n');
	}
}

char **create_array(int w, int h)
{
	char **arr = malloc(sizeof(char *) * h);
	for(int i = 0; i < h; i++)
	{
		arr[i] = malloc(w);
		for(int j = 0; j < w; j++)
		{
			arr[i][j] = ' ';
		}
	}
	return arr;
}

void free_array(char **arr, int h)
{
	for(int i = 0; i < h; i++)
		free(arr[i]);
	free(arr);
}

void draw_pattern(char **arr, int w, int h)
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
		else if (c == 's' && y < h - 1)
			y++;
		else if (c == 'a' && x > 0)
			x--;
		else if (c == 'd' && x < w - 1)
			x++;

		if (drawing && x >= 0 && x < w && y >= 0 && y < h)
			arr[y][x] = '0';
	}
}

int count_n(char **arr, int w, int h, int y, int x)
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

			if (ny >= 0 && ny < h && nx >= 0 && nx < w)
			{
				if (arr[ny][nx] == '0')
					count++;
			}
		}
	}
	return count;
}

void game_of_life(char **arr, int w, int h)
{
	char **new_arr = create_array(w, h);

	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			int n = count_n(arr, w, h, i, j);

			if (arr[i][j] == '0')
			{
				if (n == 2 || n == 3)
					new_arr[i][j] = '0';
				else
					new_arr[i][j] = ' ';
			}
			else
			{
				if (n == 3)
					new_arr[i][j] = '0';
				else
					new_arr[i][j] = ' ';
			}
		}
	}

	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			arr[i][j] = new_arr[i][j];
		}
	}

	free_array(new_arr, h);
}

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;

	int w = atoi(av[1]);
	int h = atoi(av[2]);
	int it = atoi(av[3]);

	char **arr = create_array(w, h);

	draw_pattern(arr, w, h);

	for(int i = 0; i < it; i++)
	{
		game_of_life(arr, w, h);
	}

	print_array(arr, w, h);
	free_array(arr, h);

	return 0;
}
