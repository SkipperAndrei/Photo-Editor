#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image_editor.h"

int main(void)
{
	int hist, sv;
	poza_t *poz;
	poz = alloc_memory_poza(0, 0, -1);
	char *inp;
	inp = alloc_memory_char(200);
	fgets(inp, 200, stdin);
	inp[strcspn(inp, "\n") + 1] = '\0';
	char *comm, *buf;
	comm = get_command(inp);
	while (1) {
		if (strcmp(comm, "LOAD") == 0) {
			buf = get_parameters(inp);
			FILE *in = open_the_file(buf);
			if (does_file_exist(in) == -1) {
				printf("Failed to load %s\n", buf);
				poz->color = -1;
			} else {
				poz = picture_fatality(poz);
				free(poz);
				poz = actual_fill_img(buf, in);
				fclose(in);
			}
		} else if (strcmp(comm, "SELECT") == 0) {
			poz = selection_solver(inp, poz);
		} else if (strcmp(comm, "APPLY") == 0) {
			poz = apply_solver(inp, poz, buf);
		} else if (strcmp(comm, "CROP") == 0) {
			poz = crop(poz);
		} else if (strcmp(comm, "HISTOGRAM") == 0) {
			hist = histogram_solver(poz, inp);
		} else if (strcmp(comm, "EQUALIZE") == 0) {
			poz = equalize_it(poz);
		} else if (strcmp(comm, "ROTATE") == 0) {
			poz = rotate_solver(poz, inp);
		} else if (strcmp(comm, "SAVE") == 0) {
			sv = save_file(poz, inp);
		} else if (strcmp(comm, "EXIT") == 0) {
			break;
		} else if (strcmp(comm, "\n") != 0) {
			printf("Invalid command\n");
		}
		inp[0] = '\0';
		fgets(inp, 200, stdin);
		if (inp[0] == 'E' && inp[1] == 'X' && inp[2] == 'I' && inp[3] == 'T') {
			if (!(inp[4] > 1 && inp[4] < 256))
				break;
		}
		inp[strcspn(inp, "\n") + 1] = '\0';
		free(comm);
		comm = get_command(inp);
	}
	free(comm);
	free(inp);
	if (poz->color == -1)
		printf("No image loaded\n");
	poz = picture_fatality(poz);
	free(poz);
	return 0;
}
