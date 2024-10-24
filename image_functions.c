#include <math.h>
#include <ctype.h>

typedef struct {
	int width, height;
	int **pixel;
	int color;
	int x1, x2, y1, y2;
} poza_t;

char *alloc_memory_char(int x)
{
	char *sir;
	sir = (char *)calloc(x, sizeof(char));
	if (!sir) {
		printf("Seg fault anticipat xD...\n");
		free(sir);
		exit(EXIT_FAILURE);
	}
	return sir;
}

char *get_command(char *input)
{
	input[strlen(input) - 1] = '\0';
	char *com;
	com  = alloc_memory_char(20);
	int ind = 0;
	while (input[ind] != ' ' && input[ind] != '\0' && input[ind] != '\n')
		ind++;
	strncpy(com, input, ind);
	com[ind] = '\0';
	return com;
}

char *get_parameters(char *input)
{
	char *parameters;
	parameters = strtok(input, " ");
	parameters = strtok(NULL, " ");
	return parameters;
}

int *alloc_memory_int(int x)
{
	int *tablou;
	tablou = (int *)calloc(x, sizeof(int));
	if (!tablou) {
		printf("Seg fault anticipat la int...\n");
		free(tablou);
		exit(1);
	}
	return tablou;
}

int *get_parameters_select(char *sir)
{
	int cnt = 0;
	int *v;
	v = alloc_memory_int(4);
	char *p;
	p = strtok(sir, " ");
	p = strtok(NULL, " ");
	while (p) {
		if (p[0] == '-') {
			v[cnt++] = -1;
		} else {
			if (isdigit(*p))
				v[cnt++] = atoi(p);
			else
				v[cnt++] = -2;
		}
		p = strtok(NULL, " ");
	}
	if (cnt != 4)
		v[0] = -2;
	return v;
}

int *get_parameters_histogram(char *sir)
{
	int cnt = 0;
	int *v;
	v = alloc_memory_int(5);
	char *p;
	p = strtok(sir, " ");
	while (p) {
		if (isdigit(*p))
			v[cnt++] = atoi(p);
		p = strtok(NULL, " ");
	}
	if (cnt != 2)
		v[0] = -1;
	return v;
}

int **alloc_memory_int_mat(int col, int linii)
{
	int **mat;
	mat = (int **)malloc(linii * sizeof(int *));
	if (!mat) {
		printf("Seg fauuuullllllttttttt...\n");
		free(mat);
		exit(1);
	}
	for (int i = 0 ; i < linii; i++) {
		mat[i] = (int *)malloc(col * sizeof(int));
		if (!mat[i]) {
			printf("Seq fault music to my ears...\n");
			for (int j = 0; j < i; j++)
				free(mat[j]);
			free(mat);
			exit(1);
		}
	}
	return mat;
}

int does_file_exist(FILE *f)
{
	if (!f)
		return -1;
	return 1;
}

void who_cares_about_comments(FILE *f)
{
	char a;
	char c = fgetc(f);
	while (c == '#' || isspace(c)) {
		if (c == '#') {
			a = fgetc(f);
			while (a != '\n')
				a = fgetc(f);
		}
		c = fgetc(f);
	}
	ungetc(c, f);
}

FILE *open_the_file(char *input)
{
	const char *filename = input;
	FILE *in = fopen(filename, "rb");
	return in;
}

char *interpret_the_image(FILE *f, char *input)
{
	if (does_file_exist(f) == -1) {
		printf("Failed to load %s\n", input);
		return NULL;
	}
	printf("Loaded %s\n", input);
	char *format;
	format = (char *)malloc(3 * sizeof(char));
	fread(format, 2, 1, f);
	format[2] = '\0';
	return format;
}

poza_t *alloc_memory_poza_ascii(int latime, int inaltime, int tip)
{
	poza_t *result;
	result = (poza_t *)malloc(sizeof(poza_t));
	if (!result) {
		printf("Hatz\n");
		free(result);
		exit(1);
	}
	if (tip == 1) {
		result->height = inaltime;
		result->width = latime;
		result->color = tip;
		result->x1 = 0;
		result->x2 = result->width;
		result->y1 = 0;
		result->y2 = result->height;
		return result;
	}
	result->height = inaltime;
	result->width = latime * 3;
	result->color = tip;
	result->x1 = 0;
	result->x2 = result->width;
	result->y1 = 0;
	result->y2 = result->height;
	return result;
}

poza_t *alloc_memory_poza(int latime, int inaltime, int tip)
{
	poza_t *result;
	result = (poza_t *)malloc(sizeof(poza_t));
	if (!result) {
		printf("Hatz\n");
		free(result);
		exit(1);
	}
	if (tip == 1) {
		result->pixel = alloc_memory_int_mat(latime, inaltime);
		result->height = inaltime;
		result->width = latime;
		result->color = tip;
		result->x1 = 0;
		result->x2 = result->width;
		result->y1 = 0;
		result->y2 = result->height;
		return result;
	}
	result->pixel = alloc_memory_int_mat(latime * 3, inaltime);
	result->height = inaltime;
	result->width = latime * 3;
	result->color = tip;
	result->x1 = 0;
	result->x2 = result->width;
	result->y1 = 0;
	result->y2 = result->height;
	return result;
}

int **copy_to_int(unsigned char *sir, int width, int height)
{
	int **result;
	int cnt = 0;
	result = alloc_memory_int_mat(width, height);
	for (int i = 0 ; i < height; i++)
		for (int j = 0 ; j < width; j++)
			result[i][j] = sir[cnt++];
	return result;
}

poza_t *actual_fill_img(char *input, FILE *input_file)
{
	unsigned char *v;
	poza_t *poza_rezultat = NULL;
	char *format;
	format = interpret_the_image(input_file, input);
	if (!format) {
		free(format);
		return poza_rezultat;
	}
	int latime, inaltime, maximum;
	who_cares_about_comments(input_file);
	fscanf(input_file, "%d %d", &latime, &inaltime);
	who_cares_about_comments(input_file);
	fscanf(input_file, "%d", &maximum);
	who_cares_about_comments(input_file);
	if (strcmp(format, "P2") == 0 || strcmp(format, "P3") == 0) {
		if (format[1] == '2') {
			poza_rezultat = alloc_memory_poza_ascii(latime, inaltime, 1);
			poza_rezultat->color = 12;
			int l = latime, h = inaltime;
			v = (unsigned char *)calloc(l * h, sizeof(unsigned char));
			if (!v) {
				printf("Dog\n");
				free(v);
				exit(1);
			}
			for (int i = 0 ; i < latime * inaltime; i++)
				fscanf(input_file, "%hhu", &v[i]);
			poza_rezultat->pixel = copy_to_int(v, latime, inaltime);
		} else {
			poza_rezultat = alloc_memory_poza_ascii(latime, inaltime, 0);
			poza_rezultat->color = 3;
			latime *= 3;
			v = (unsigned char *)calloc(latime * inaltime, 1);
			for (int i = 0; i < latime * inaltime; i++)
				fscanf(input_file, "%hhu", &v[i]);
			poza_rezultat->pixel = copy_to_int(v, latime, inaltime);
		}
	} else {
		if (format[1] == '5') {
			poza_rezultat = alloc_memory_poza(latime, inaltime, 1);
			poza_rezultat->color = 15;
			int l = latime, h = inaltime;
			v = (unsigned char *)calloc(l * h, sizeof(unsigned char));
			if (!v) {
				printf("Dog\n");
				free(v);
				exit(1);
			}
			for (int i = 0; i < inaltime; i++) {
				fread(v, sizeof(unsigned char), latime, input_file);
				for (int j = 0; j < latime; j++)
					poza_rezultat->pixel[i][j] = (int)v[j];
			}
		} else if (format[1] == '6') {
			poza_rezultat = alloc_memory_poza(latime, inaltime, 0);
			poza_rezultat->color = 6;
			latime *= 3;
			v = (unsigned char *)calloc(latime * inaltime, 1);
			for (int i = 0; i < inaltime; i++) {
				fread(v, sizeof(unsigned char), latime, input_file);
				for (int j = 0; j < latime; j++)
					poza_rezultat->pixel[i][j] = (int)v[j];
			}
		}
	}
	free(format);
	free(v);
	return poza_rezultat;
}

poza_t *picture_fatality(poza_t *p)
{
	for (int i = 0 ; i < p->height; i++)
		free(p->pixel[i]);
	free(p->pixel);
	p->color = -1;
	return p;
}

poza_t *copy_sel(poza_t *poz, int c[])
{
	if (c[0] > c[2]) {
		poz->x1 = c[2];
		poz->x2 = c[0];
	} else {
		poz->x1 = c[0];
		poz->x2 = c[2];
	}
	if (c[1] > c[3]) {
		poz->y1 = c[3];
		poz->y2 = c[1];
	} else {
		poz->y1 = c[1];
		poz->y2 = c[3];
	}
	return poz;
}

poza_t *selection_solver(char *input, poza_t *poz)
{
	if (poz->color == -1) {
		printf("No image loaded\n");
		return poz;
	}
	if (input[7] == 'A' && input[8] == 'L' && input[9] == 'L') {
		printf("Selected ALL\n");
		poz->x1 = 0;
		poz->y1 = 0;
		poz->x2 = poz->width;
		poz->y2 = poz->height;
		return poz;
	}
	int valid_input = 1, *coord;
	coord = get_parameters_select(input);
	if (coord[0] == -2 || coord[1] == -2 || coord[2] == -2 || coord[3] == -2) {
		printf("Invalid command\n");
		free(coord);
		return poz;
	}
	if (poz->color > 10) {
		if (coord[0] < 0 || coord[0] > poz->width)
			valid_input = 0;
		if (coord[1] < 0 || coord[1] > poz->height)
			valid_input = 0;
		if (coord[2] < 0 || coord[2] == coord[0] || coord[2] > poz->width)
			valid_input = 0;
		if (coord[3] < 0 || coord[3] == coord[1] || coord[3] > poz->height)
			valid_input = 0;
	} else {
		if (coord[0] < 0 || 3 * coord[0] > poz->width)
			valid_input = 0;
		if (coord[1] < 0 || coord[1] > poz->height)
			valid_input = 0;
		if (coord[2] < 0 || coord[2] == coord[0] || 3 * coord[2] > poz->width)
			valid_input = 0;
		if (coord[3] < 0 || coord[3] == coord[1] || coord[3] > poz->height)
			valid_input = 0;
	}
	if (valid_input == 1) {
		copy_sel(poz, coord);
		printf("Selected %d %d %d %d\n", poz->x1, poz->y1, poz->x2, poz->y2);
		if (poz->color == 3 || poz->color == 6) {
			coord[0] *= 3;
			coord[2] *= 3;
			poz = copy_sel(poz, coord);
		}
	} else {
		printf("Invalid set of coordinates\n");
	}
	free(coord);
	return poz;
}

int clamp(int x, int min, int max)
{
	if (x > max)
		return max;
	if (x < min)
		return min;
	return x;
}

void freecolors(int **mat, int lin)
{
	for (int i = 0; i < lin; i++)
		free(mat[i]);
	free(mat);
}

int check_interval(int a, int b, int x)
{
	if (x >= a && x < b)
		return 1;
	return 0;
}

int **apply_sharpen(int **mat, int col, int lin, poza_t *poz)
{
	int **result = alloc_memory_int_mat(col, lin);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			result[i][j] = 0;
	int sh[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++) {
			if (check_interval(poz->y1, poz->y2, i) == 1) {
				if (check_interval(poz->x1 / 3, poz->x2 / 3, j) == 1) {
					if (i != 0 && i != lin - 1 && j != 0 && j != col - 1) {
						result[i][j] = sh[0][0] * mat[i - 1][j - 1]
						+ sh[0][1] * mat[i - 1][j]
						+ sh[0][2] * mat[i - 1][j + 1]
						+ sh[1][0] * mat[i][j - 1]
						+ sh[1][1] * mat[i][j]
						+ sh[1][2] * mat[i][j + 1]
						+ sh[2][0] * mat[i + 1][j - 1]
						+ sh[2][1] * mat[i + 1][j]
						+ sh[2][2] * mat[i + 1][j + 1];
						result[i][j] = clamp(result[i][j], 0, 255);
					} else {
						result[i][j] = mat[i][j];
					}
				} else {
					result[i][j] = mat[i][j];
				}
			} else {
				result[i][j] = mat[i][j];
			}
		}
	return result;
}

poza_t *sharp(poza_t *poz)
{
	int lin = poz->y2 - poz->y1;
	int col1 = poz->x1 / 3, col2 = poz->x2 / 3;
	int col = col2 - col1;
	int **color, **result;
	color = alloc_memory_int_mat(poz->width / 3, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_sharpen(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_sharpen(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_sharpen(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(color, poz->height);
	freecolors(result, poz->height);
	return poz;
}

int **apply_edge(int **mat, int col, int lin, poza_t *poz)
{
	int **result = alloc_memory_int_mat(col, lin);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			result[i][j] = 0;
	int ed[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++) {
			if (check_interval(poz->y1, poz->y2, i) == 1) {
				if (check_interval(poz->x1 / 3, poz->x2 / 3, j) == 1) {
					if (i != 0 && i != lin - 1 && j != 0 && j != col - 1) {
						result[i][j] = ed[0][0] * mat[i - 1][j - 1]
						+ ed[0][1] * mat[i - 1][j]
						+ ed[0][2] * mat[i - 1][j + 1]
						+ ed[1][0] * mat[i][j - 1] + ed[1][1] * mat[i][j]
						+ ed[1][2] * mat[i][j + 1]
						+ ed[2][0] * mat[i + 1][j - 1]
						+ ed[2][1] * mat[i + 1][j]
						+ ed[2][2] * mat[i + 1][j + 1];
						result[i][j] = clamp(result[i][j], 0, 255);
					} else {
						result[i][j] = mat[i][j];
					}
				} else {
					result[i][j] = mat[i][j];
				}
			} else {
				result[i][j] = mat[i][j];
			}
		}
	return result;
}

poza_t *edge(poza_t *poz)
{
	int **color, **result;
	color = alloc_memory_int_mat(poz->width / 3, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_edge(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_edge(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_edge(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(color, poz->height);
	freecolors(result, poz->height);
	return poz;
}

int **apply_blur(int **mat, int col, int lin, poza_t *poz)
{
	double blur[3][3] = {{(1.0 / 9), (1.0 / 9), (1.0 / 9)},
	{(1.0 / 9), (1.0 / 9), (1.0 / 9)}, {(1.0 / 9), (1.0 / 9), (1.0 / 9)}};
	int **result = alloc_memory_int_mat(col, lin);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			result[i][j] = 0;
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++) {
			if (check_interval(poz->y1, poz->y2, i) == 1) {
				if (check_interval(poz->x1 / 3, poz->x2 / 3, j) == 1) {
					if (i != 0 && i != lin - 1 && j != 0 && j != col - 1) {
						result[i][j] = round(blur[0][0] * mat[i - 1][j - 1]
						+ blur[0][1] * mat[i - 1][j]
						+ blur[0][2] * mat[i - 1][j + 1]
						+ blur[1][0] * mat[i][j - 1] + blur[1][1] * mat[i][j]
						+ blur[1][2] * mat[i][j + 1]
						+ blur[2][0] * mat[i + 1][j - 1]
						+ blur[2][1] * mat[i + 1][j]
						+ blur[2][2] * mat[i + 1][j + 1]);
						result[i][j] = clamp(result[i][j], 0, 255);
					} else {
						result[i][j] = mat[i][j];
					}
				} else {
					result[i][j] = mat[i][j];
				}
			} else {
				result[i][j] = mat[i][j];
			}
		}
	return result;
}

poza_t *blurring(poza_t *poz)
{
	int **color, **result;
	color = alloc_memory_int_mat(poz->width / 3, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_blur(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_blur(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_blur(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(color, poz->height);
	freecolors(result, poz->height);
	return poz;
}

int **apply_gauss(int **mat, int col, int lin, poza_t *poz)
{
	int **result = alloc_memory_int_mat(col, lin);
	double gauss[3][3] = {{0.0625, 0.125, 0.0625},
	{0.125, 0.25, 0.125}, {0.0625, 0.125, 0.0625}};
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			result[i][j] = 0;
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++) {
			if (check_interval(poz->y1, poz->y2, i) == 1) {
				if (check_interval(poz->x1 / 3, poz->x2 / 3, j) == 1) {
					if (i != 0 && i != lin - 1 && j != 0 && j != col - 1) {
						result[i][j] = round(gauss[0][0] * mat[i - 1][j - 1]
						+ gauss[0][1] * mat[i - 1][j]
						+ gauss[0][2] * mat[i - 1][j + 1]
						+ gauss[1][0] * mat[i][j - 1] + gauss[1][1] * mat[i][j]
						+ gauss[1][2] * mat[i][j + 1]
						+ gauss[2][0] * mat[i + 1][j - 1]
						+ gauss[2][1] * mat[i + 1][j]
						+ gauss[2][2] * mat[i + 1][j + 1]);
						result[i][j] = clamp(result[i][j], 0, 255);
					} else {
						result[i][j] = mat[i][j];
					}
				} else {
					result[i][j] = mat[i][j];
				}
			} else {
				result[i][j] = mat[i][j];
			}
		}
	return result;
}

poza_t *gauss(poza_t *poz)
{
	int **color, **result;
	color = alloc_memory_int_mat(poz->width / 3, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_gauss(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 0)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_gauss(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 1)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(result, poz->height);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				color[i][j / 3] = poz->pixel[i][j];
	result = apply_gauss(color, poz->width / 3, poz->height, poz);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			if (j % 3 == 2)
				poz->pixel[i][j] = result[i][j / 3];
	freecolors(color, poz->height);
	freecolors(result, poz->height);
	return poz;
}

poza_t *apply_solver(char *input, poza_t *poza, char *param)
{
	if (poza->color == -1) {
		printf("No image loaded\n");
		return poza;
	}
	if (strcmp(input, "APPLY") == 0) {
		printf("Invalid command\n");
		return poza;
	}
	if (poza->color > 10) {
		printf("Easy, Charlie Chaplin\n");
		return poza;
	}
	param = get_parameters(input);
	if (strcmp(param, "SHARPEN") == 0) {
		printf("APPLY SHARPEN done\n");
		poza = sharp(poza);
	} else if (strcmp(param, "EDGE") == 0) {
		printf("APPLY EDGE done\n");
		poza = edge(poza);
	} else if (strcmp(param, "GAUSSIAN_BLUR") == 0) {
		printf("APPLY GAUSSIAN_BLUR done\n");
		poza = gauss(poza);
	} else if (strcmp(param, "BLUR") == 0) {
		printf("APPLY BLUR done\n");
		poza = blurring(poza);
	} else {
		printf("APPLY parameter invalid\n");
	}
	return poza;
}

poza_t *crop(poza_t *poza)
{
	if (poza->color == -1) {
		printf("No image loaded\n");
		return poza;
	}
	if (poza->x1 == 0 && poza->x2 == poza->width &&
		poza->y1 == 0 && poza->y2 == poza->height) {
		printf("Image cropped\n");
		return poza;
	}
	int cod_format = poza->color;
	int new_width = poza->x2 - poza->x1;
	int new_height = poza->y2 - poza->y1;
	int **new_pixel = alloc_memory_int_mat(new_width, new_height);
	for (int i = 0; i < new_height; i++)
		for (int j = 0; j < new_width; j++)
			new_pixel[i][j] = poza->pixel[i + poza->y1][j + poza->x1];
	poza = picture_fatality(poza);
	poza->pixel = new_pixel;
	poza->width = new_width;
	poza->height = new_height;
	poza->x1 = 0;
	poza->x2 = new_width;
	poza->y1 = 0;
	poza->y2 = new_height;
	poza->color = cod_format;
	printf("Image cropped\n");
	return poza;
}

int verify_power(int x)
{
	int power_two[9] = {0, 2, 4, 8, 16, 32, 64, 128, 256};
	for (int i = 0; i < 9; i++)
		if (x == power_two[i])
			return 1;
	return 0;
}

int *fill_the_freq(poza_t *poz, int x, int y)
{
	int *freq;
	freq = alloc_memory_int(y);
	int dif = 256 / y;
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			freq[poz->pixel[i][j] / dif] += 1;
	return freq;
}

int find_maximum(int *v, int dim)
{
	int max = -1;
	for (int i = 0; i < dim; i++)
		if (v[i] > max)
			max = v[i];
	return max;
}

int histogram_solver(poza_t *poza, char *sir)
{
	if (poza->color == -1) {
		printf("No image loaded\n");
		return 0;
	}
	int *param;
	param = get_parameters_histogram(sir);
	if (param[0] == -1) {
		printf("Invalid command\n");
		free(param);
		return 0;
	}
	if (poza->color < 10) {
		printf("Black and white image needed\n");
		free(param);
		return 0;
	}
	if (verify_power(param[1]) == 0) {
		printf("Invalid set of parameters\n");
		free(param);
		return 0;
	}
	int *distrib_stel;
	distrib_stel = fill_the_freq(poza, param[0], param[1]);
	int max = -1;
	max = find_maximum(distrib_stel, param[1]);
	for (int i = 0; i < param[1]; i++) {
		if (distrib_stel[i] == 0) {
			printf("0\t|\t\n");
		} else {
			int nr_stel = floor(distrib_stel[i] * 1.0 / max * param[0]);
			printf("%d\t|\t", nr_stel);
			for (int j = 0; j < nr_stel; j++)
				printf("*");
			printf("\n");
		}
	}
	free(param);
	free(distrib_stel);
	return 1;
}

poza_t *equalize_it(poza_t *poz)
{
	if (poz->color == -1) {
		printf("No image loaded\n");
		return poz;
	}
	if (poz->color < 10) {
		printf("Black and white image needed\n");
		return poz;
	}
	printf("Equalize done\n");
	int sum = 0;
	int area = poz->width * poz->height;
	int his[256] = {0}, valori_noi[256] = {0};
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			his[poz->pixel[i][j]] += 1;
	for (int i = 0; i < 256; i++) {
		sum += his[i];
		valori_noi[i] = round((double)255 * sum / area);
		valori_noi[i] = clamp(valori_noi[i], 0, 255);
	}
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			poz->pixel[i][j] = valori_noi[poz->pixel[i][j]];
	return poz;
}

int get_parameters_rotate(char *input)
{
	int unghi;
	char *buffer;
	buffer = alloc_memory_char(5);
	strcpy(buffer, input + 7);
	unghi = atoi(buffer);
	free(buffer);
	return unghi;
}

int verify_angle(int x)
{
	int good_angles[9] = {-360, -270, -180, -90, 0, 90, 180, 270, 360};
	for (int i = 0; i < 9; i++)
		if (x == good_angles[i])
			return 1;
	return 0;
}

int check_square(poza_t *poz)
{
	int lat1 = poz->y2 - poz->y1;
	int lat2;
	if (poz->color < 10)
		lat2 = (poz->x2 - poz->x1) / 3;
	else
		lat2 = poz->x2 - poz->x1;
	if (lat1 != lat2)
		return 0;
	return 1;
}

int neg(int x)
{
	if (x >= 0)
		return x;
	return (-1) * x;
}

poza_t *rotate_pic(poza_t *poz)
{
	int lat1 = poz->y2 - poz->y1;
	int lat2 = poz->x2 - poz->x1;
	int col;
	if (poz->x1 != 0 || poz->x2 != poz->width ||
		poz->y1 != 0 || poz->y2 != poz->height) {
		int **mat;
		mat = alloc_memory_int_mat(lat2, lat1);
		for (int i = poz->y1; i < poz->y2; i++) {
			col = 0;
			for (int j = poz->x1; j < poz->x2; j++)
				mat[i - poz->y1][col++] = poz->pixel[i][j];
		}
		for (int i = 0; i < lat1; i++)
			for (int j = 0; j < i; j++) {
				int tmp = mat[i][j];
				mat[i][j] = mat[j][i];
				mat[j][i] = tmp;
			}
		for (int i = 0; i < lat1 / 2; i++)
			for (int j = 0; j < lat2; j++) {
				int tmp = mat[i][j];
				mat[i][j] = mat[lat1 - i - 1][j];
				mat[lat1 - i - 1][j] = tmp;
			}
		for (int i = poz->y1; i < poz->y2; i++)
			for (int j = poz->x1; j < poz->x2; j++)
				poz->pixel[i][j] = mat[i - poz->y1][j - poz->x1];
		freecolors(mat, lat1);
		return poz;
	}
	int **mat;
	mat = alloc_memory_int_mat(poz->height, poz->width);
	for (int i = 0; i < poz->height; i++)
		for (int j = 0; j < poz->width; j++)
			mat[j][i] = poz->pixel[i][j];
	for (int i = 0; i < poz->width / 2; i++)
		for (int j = 0; j < poz->height; j++) {
			int tmp = mat[i][j];
			mat[i][j] = mat[poz->width - i - 1][j];
			mat[poz->width - i - 1][j] = tmp;
		}
	int format = poz->color;
	picture_fatality(poz);
	poz->color = format;
	poz->pixel = mat;
	poz->x1 = 0;
	poz->x2 = poz->height;
	poz->y1 = 0;
	poz->y2 = poz->width;
	poz->height = poz->y2;
	poz->width = poz->x2;
	return poz;
}

poza_t *rotate_pic_rgb_frag(poza_t *poz)
{
	int lat1 = poz->y2 - poz->y1;
	int lat2 = (poz->x2 - poz->x1) / 3;
	int col;
	typedef struct {
		int red, green, blue;
	} pix_t;
	pix_t **mat;
	mat = (pix_t **)malloc(lat1 * sizeof(pix_t *));
	if (!mat) {
		fprintf(stderr, "Memory issue not skill\n");
		free(mat);
		exit(1);
	}
	for (int i = 0; i < lat1; i++) {
		mat[i] = (pix_t *)malloc(lat1 * sizeof(pix_t));
		if (!mat[i]) {
			fprintf(stderr, "Memory issue not skill\n");
			for (int j = 0; j < i; j++)
				free(mat[j]);
			free(mat);
			exit(1);
		}
	}
	for (int i = poz->y1; i < poz->y2; i++)
		for (int j = poz->x1 / 3; j < poz->x2 / 3; j++) {
			mat[i - poz->y1][j - poz->x1 / 3].red = poz->pixel[i][3 * j];
			mat[i - poz->y1][j - poz->x1 / 3].green = poz->pixel[i][3 * j + 1];
			mat[i - poz->y1][j - poz->x1 / 3].blue = poz->pixel[i][3 * j + 2];
		}
	for (int i = 0; i < lat1; i++)
		for (int j = 0; j < i; j++) {
			pix_t tmp = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = tmp;
		}
	for (int i = 0; i < lat1 / 2; i++)
		for (int j = 0; j < lat2; j++) {
			pix_t tmp = mat[i][j];
			mat[i][j] = mat[lat1 - i - 1][j];
			mat[lat1 - i - 1][j] = tmp;
		}
	for (int i = poz->y1; i < poz->y2; i++)
		for (int j = poz->x1 / 3; j < poz->x2 / 3; j++) {
			poz->pixel[i][3 * j] = mat[i - poz->y1][j - poz->x1 / 3].red;
			poz->pixel[i][3 * j + 1] = mat[i - poz->y1][j - poz->x1 / 3].green;
			poz->pixel[i][3 * j + 2] = mat[i - poz->y1][j - poz->x1 / 3].blue;
		}
	for (int i = 0; i < lat1; i++)
		free(mat[i]);
	free(mat);
	return poz;
}

poza_t *rotate_pic_rgb_full(poza_t *poz)
{
	int lat1 = poz->height, lat2 = poz->width / 3;
	typedef struct {
		int red, green, blue;
	} pix_t;
	pix_t **mat;
	mat = (pix_t **)malloc(lat1 * sizeof(pix_t *));
	if (!mat) {
		fprintf(stderr, "Memory issue not skill\n");
		free(mat);
		exit(1);
	}
	for (int i = 0; i < lat1; i++) {
		mat[i] = (pix_t *)malloc(lat2 * sizeof(pix_t));
		if (!mat[i]) {
			fprintf(stderr, "Memory issue not skill\n");
			for (int j = 0; j < i; j++)
				free(mat[j]);
			free(mat);
			exit(1);
		}
	}
	for (int i = 0; i < lat1; i++)
		for (int j = 0; j < lat2; j++) {
			mat[i][j].red = poz->pixel[i][3 * j];
			mat[i][j].green = poz->pixel[i][3 * j + 1];
			mat[i][j].blue = poz->pixel[i][3 * j + 2];
		}
	int format = poz->color;
	poz = picture_fatality(poz);
	poz->color = format;
	pix_t **result;
	result = (pix_t **)malloc(lat2 * sizeof(pix_t *));
	if (!result) {
		fprintf(stderr, "Memory issue not skill\n");
		free(result);
		exit(1);
	}
	for (int i = 0; i < lat2; i++) {
		result[i] = (pix_t *)malloc(lat1 * sizeof(pix_t));
		if (!result[i]) {
			fprintf(stderr, "Memory issue not skill\n");
			for (int j = 0; j < i; j++)
				free(result[j]);
			free(result);
			exit(1);
		}
	}
	for (int i = 0 ; i < lat1; i++)
		for (int j = 0; j < lat2; j++)
			result[j][i] = mat[i][j];
	for (int i = 0; i < lat1; i++)
		free(mat[i]);
	free(mat);
	for (int i = 0; i < lat2 / 2; i++)
		for (int j = 0; j < lat1; j++) {
			pix_t tmp = result[i][j];
			result[i][j] = result[lat2 - i - 1][j];
			result[lat2 - i - 1][j] = tmp;
		}
	poz->pixel = alloc_memory_int_mat(3 * lat1, lat2);
	for (int i = 0; i < lat2; i++)
		for (int j = 0; j < lat1; j++) {
			poz->pixel[i][3 * j] = result[i][j].red;
			poz->pixel[i][3 * j + 1] = result[i][j].green;
			poz->pixel[i][3 * j + 2] = result[i][j].blue;
		}
	poz->height = lat2;
	poz->width = 3 * lat1;
	poz->x1 = 0;
	poz->y1 = 0;
	poz->x2 = poz->width;
	poz->y2 = poz->height;
	for (int i = 0; i < lat2; i++)
		free(result[i]);
	free(result);
	return poz;
}

poza_t *rotate_solver(poza_t *poz, char *input)
{
	if (poz->color == -1) {
		printf("No image loaded\n");
		return poz;
	}
	int angle = get_parameters_rotate(input);
	if (verify_angle(angle) == 0) {
		printf("Unsupported rotation angle\n");
		return poz;
	}
	if (poz->x1 != 0 && poz->x2 != poz->height &&
		poz->y1 != 0 && poz->y2 != poz->height) {
		if (check_square(poz) == 0) {
			printf("Selection must be square\n");
			return poz;
		}
	}
	if (strcmp(input, "ROTATE") == 0) {
		printf("Invalid command\n");
		return poz;
	}
	printf("Rotated %d\n", angle);
	if (poz->color > 10) {
		if (angle > 0)
			for (int i = 0; i < (360 - angle) / 90; i++)
				poz = rotate_pic(poz);
		else
			for (int i = 0; i < neg(angle) / 90; i++)
				poz = rotate_pic(poz);
	} else {
		if (angle > 0) {
			if (poz->y1 == 0 && poz->y2 == poz->height &&
				poz->x1 == 0 && poz->x2 == poz->width)
				for (int i = 0; i < (360 - angle) / 90; i++)
					poz = rotate_pic_rgb_full(poz);
			else
				for (int i = 0; i < (360 - angle) / 90; i++)
					poz = rotate_pic_rgb_frag(poz);
		} else {
			if (poz->y1 == 0 && poz->y2 == poz->height &&
				poz->x1 == 0 && poz->x2 == poz->width)
				for (int i = 0; i < neg(angle) / 90; i++)
					poz = rotate_pic_rgb_full(poz);
			else
				for (int i = 0; i < neg(angle) / 90; i++)
					poz = rotate_pic_rgb_frag(poz);
		}
	}
	return poz;
}

int is_ascii_or_not(char *input)
{
	int length = strlen(input);
	if (input[length - 1] == 'i' && input[length - 2] == 'i' &&
		input[length - 3] == 'c' && input[length - 4] == 's' &&
		input[length - 5] == 'a')
		return 1;
	return 0;
}

char *get_filename(char *input)
{
	char *token;
	token = strtok(input, " ");
	token = strtok(NULL, " ");
	return token;
}

int save_file(poza_t *poz, char *input)
{
	if (poz->color == -1) {
		printf("No image loaded\n");
		return 0;
	}
	int txt_or_bin = is_ascii_or_not(input);
	char *filename;
	filename = get_filename(input);
	printf("Saved %s\n", filename);
	if (txt_or_bin == 1) {
		FILE *out = fopen(filename, "w");
		if (poz->color == 3 || poz->color == 6) {
			fprintf(out, "P3\n");
			fprintf(out, "%d %d\n", poz->width / 3, poz->height);
		} else {
			fprintf(out, "P2\n");
			fprintf(out, "%d %d\n", poz->width, poz->height);
		}
		fprintf(out, "255\n");
		for (int i = 0; i < poz->height; i++) {
			for (int j = 0; j < poz->width; j++)
				fprintf(out, "%d ", poz->pixel[i][j]);
			fprintf(out, "\n");
		}
		fclose(out);
		return 1;
	}
	int wide;
	FILE *out = fopen(filename, "wb");
	if (poz->color == 3 || poz->color == 6) {
		fprintf(out, "P6\n");
		fprintf(out, "%d %d\n", poz->width / 3, poz->height);
		fprintf(out, "255\n");
		for (int i = 0; i < poz->height; i++)
			for (int j = 0; j < poz->width; j++) {
				unsigned char valori[2];
				valori[0] = (unsigned char)poz->pixel[i][j];
				fwrite(valori, sizeof(unsigned char), 1, out);
			}
	} else {
		fprintf(out, "P5\n");
		fprintf(out, "%d %d\n", poz->width, poz->height);
		fprintf(out, "255\n");
		for (int i = 0; i < poz->height; i++)
			for (int j = 0; j < poz->width; j++) {
				unsigned char valori[2];
				valori[0] = (unsigned char)poz->pixel[i][j];
				fwrite(valori, sizeof(unsigned char), 1, out);
			}
	}
	fclose(out);
	return 1;
}
