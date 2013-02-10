#ifndef CONTEST_H_INCLUDED
#define CONTEST_H_INCLUDED

#include <stdio.h>
#include <inttypes.h>

struct contest_operand_t {
	unsigned char A;
	unsigned char B;
	unsigned char C;
	unsigned char X;
};

struct contest_data_t {
	int	width;
	int	height;
	struct contest_operand_t	*data;
};

struct contest_data_t*
bmpread_read(
	FILE *fbmp );

void
contest_parse(
	struct contest_data_t *data );

void
draw_plot_line(
	const int x0,
	const int y0,
	const int x1,
	const int y1 );


#endif
