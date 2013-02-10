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

#define	BMPREAD_HDR_SIZE	54
#define	DRAW_SCALE	1

struct contest_data_t {
	unsigned char	hdr[ BMPREAD_HDR_SIZE ];
	unsigned int	width;
	unsigned int	height;
	unsigned int	size_src;
	unsigned int	size_res;
	struct contest_operand_t	*data;
	struct contest_operand_t	*result;
};

void
contest_error(
	const char * const msg );

struct contest_data_t*
bmpread_read(
	FILE * const fbmp );

void
bmpread_save(
	FILE * const fres,
	const struct contest_data_t * const bmp );

void
bmpread_free(
	struct contest_data_t * const bmp );

struct contest_operand_t*
bmpread_get_opcode(
	const struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y );

void
contest_parse(
	const struct contest_data_t * const bmp );

void
draw_line(
	const struct contest_data_t * const bmp,
	const int xxs,
	const int yys,
	const int xxe,
	const int yye,
	const signed char color );


#endif
