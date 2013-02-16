#ifndef CONTEST_H_INCLUDED
#define CONTEST_H_INCLUDED

#include <stdio.h>
#include <inttypes.h>

struct contest_operand_t {
	signed char A;
	signed char B;
	signed char C;
};

#define	BMPREAD_HDR_SIZE	54
#define	DRAW_SCALE	1

struct contest_data_t {
	unsigned char	hdr[ BMPREAD_HDR_SIZE ];
	unsigned int	width;
	unsigned int	height;
	unsigned int	count;
	unsigned int	size_src;
	unsigned int	size_res;
	struct contest_operand_t	*data;
	struct contest_operand_t	*result_color;
	struct contest_operand_t	*result_bw;
	struct contest_operand_t	*result_used;
};

void
contest_error(
	const char * const msg );

struct contest_data_t*
bmpread_read(
	FILE * const fbmp );

void
bmpread_save(
	const struct contest_data_t * const bmp,
	const char * const fdir,
	const char * const fname );

void
bmpread_alloc_bw(
	struct contest_data_t * const bmp );

void
bmpread_free(
	struct contest_data_t * const bmp );

void
bmpread_fill_bw_from_color(
	struct contest_data_t * const bmp );

void
bmpread_fill_bw_from_used(
	struct contest_data_t * const bmp );

struct contest_operand_t*
bmpread_get_opcode(
	const struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y );

void
contest_parse(
	struct contest_data_t * const bmp,
	const char * const fdir );

void
draw_line(
	const struct contest_data_t * const bmp,
	struct contest_operand_t * const pixels,
	const int xxs,
	const int yys,
	const int xxe,
	const int yye,
	const signed char color );

#endif
