#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct s_map {
	char **grid;
	int **dp;
	int rows;
	int cols;
	char empty;
	char obstacle;
	char full;
} t_map;

typedef struct s_square {
	int size;
	int row;
	int col;
} t_square;

int min3(int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	return min;
}

void free_map(t_map *map)
{
	if (!map)
		return;

	if (map->grid)
	{
		for (int i = 0; i < map->rows; i++)
			free(map->grid[i]);
		free(map->grid);
	}

	if (map->dp)
	{
		for (int i = 0; i < map->rows; i++)
			free(map->dp[i]);
		free(map->dp);
	}
}

int parse_first_line(FILE *fp, t_map *map)
{
	if (fscanf(fp, "%d%c%c%c\n", &map->rows, &map->empty, &map->obstacle, &map->full) != 4)
		return 0;

	if (map->rows < 1)
		return 0;

	if (map->empty == map->obstacle || map->empty == map->full || map->obstacle == map->full)
		return 0;

	return 1;
}

int read_map(FILE *fp, t_map *map)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int i = 0;

	map->cols = -1;
	map->grid = malloc(sizeof(char *) * map->rows);
	if (!map->grid)
		return 0;

	while ((read = getline(&line, &len, fp)) != -1 && i < map->rows)
	{
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';
		else
			read++;

		if (map->cols == -1)
			map->cols = read - 1;
		else if (map->cols != read - 1)
		{
			free(line);
			return 0;
		}

		if (map->cols < 1)
		{
			free(line);
			return 0;
		}

		map->grid[i] = malloc(map->cols + 1);
		if (!map->grid[i])
		{
			free(line);
			return 0;
		}

		for (int j = 0; j < map->cols; j++)
		{
			if (line[j] != map->empty && line[j] != map->obstacle)
			{
				free(line);
				return 0;
			}
			map->grid[i][j] = line[j];
		}
		map->grid[i][map->cols] = '\0';
		i++;
	}

	free(line);

	if (i != map->rows)
		return 0;

	return 1;
}

int create_dp_table(t_map *map)
{
	map->dp = malloc(sizeof(int *) * map->rows);
	if (!map->dp)
		return 0;

	for (int i = 0; i < map->rows; i++)
	{
		map->dp[i] = malloc(sizeof(int) * map->cols);
		if (!map->dp[i])
			return 0;

		for (int j = 0; j < map->cols; j++)
		{
			map->dp[i][j] = 0;
		}
	}
	return 1;
}

t_square find_biggest_square(t_map *map)
{
	t_square square = {0, 0, 0};

	for (int i = 0; i < map->rows; i++)
	{
		for (int j = 0; j < map->cols; j++)
		{
			if (map->grid[i][j] == map->obstacle)
			{
				map->dp[i][j] = 0;
			}
			else if (i == 0 || j == 0)
			{
				map->dp[i][j] = 1;
			}
			else
			{
				map->dp[i][j] = min3(map->dp[i-1][j], map->dp[i][j-1], map->dp[i-1][j-1]) + 1;
			}

			if (map->dp[i][j] > square.size)
			{
				square.size = map->dp[i][j];
				square.row = i;
				square.col = j;
			}
		}
	}

	return square;
}

void fill_square(t_map *map, t_square square)
{
	int start_row = square.row - square.size + 1;
	int start_col = square.col - square.size + 1;

	for (int i = start_row; i <= square.row; i++)
	{
		for (int j = start_col; j <= square.col; j++)
		{
			map->grid[i][j] = map->full;
		}
	}
}

void print_map(t_map *map)
{
	for (int i = 0; i < map->rows; i++)
	{
		printf("%s\n", map->grid[i]);
	}
}

int process_file(FILE *fp)
{
	t_map map = {NULL, NULL, 0, 0, 0, 0, 0};

	if (!parse_first_line(fp, &map))
	{
		printf("Error: invalid map\n");
		return 0;
	}

	if (!read_map(fp, &map))
	{
		printf("Error: invalid map\n");
		free_map(&map);
		return 0;
	}

	if (!create_dp_table(&map))
	{
		printf("Error: memory allocation failed\n");
		free_map(&map);
		return 0;
	}

	t_square square = find_biggest_square(&map);

	if (square.size > 0)
		fill_square(&map, square);

	print_map(&map);
	free_map(&map);

	return 1;
}

int main(int ac, char **av)
{
	if (ac == 1)
	{
		process_file(stdin);
	}
	else
	{
		for (int i = 1; i < ac; i++)
		{
			FILE *fp = fopen(av[i], "r");
			if (!fp)
			{
				printf("Error: cannot open file %s\n", av[i]);
				continue;
			}

			process_file(fp);
			fclose(fp);

			if (i < ac - 1)
				printf("\n");
		}
	}

	return 0;
}
