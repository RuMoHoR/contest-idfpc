#ifndef CONTEST_H_INCLUDED
#define CONTEST_H_INCLUDED

#include <stdio.h>
#include <inttypes.h>

struct contest_operand_t {
	signed char A;
	signed char B;
	signed char C;
//	signed char X;
};

struct contest_data_t {
	unsigned int	width;
	unsigned int	height;
	unsigned int	limit;
	struct contest_operand_t	*data;
};

void
contest_error(
	const char * const msg );

struct contest_data_t*
bmpread_read(
	FILE * const fbmp );

struct contest_operand_t*
bmpread_get_opcode(
	const struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y );

void
contest_parse(
	const struct contest_data_t * const data );

void
draw_plot_line(
	const int x0,
	const int y0,
	const int x1,
	const int y1 );


#endif
