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
	unsigned int	size;
	struct contest_operand_t	*data;
	struct contest_operand_t	*result_color;
	struct contest_operand_t	*result_bw;
	struct contest_operand_t	*result_used;
};

struct contest_state_t {
	unsigned int	x;
	unsigned int	y;
	signed char	vx;
	signed char	vy;
	signed char	clr;
	int	dump;
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
	const char * const fname,
	const struct contest_operand_t *result );

void
bmpread_alloc_bw(
	struct contest_data_t * const bmp );

void
bmpread_free(
	struct contest_data_t * const bmp );

struct contest_operand_t*
bmpread_get_opcode(
	const struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y );

void
contest_dump(
	const char * const label,
	const struct contest_state_t * const state,
	const struct contest_operand_t * const op );

void
contest_dump_start(
	struct contest_state_t * const state );

void
contest_dump_stop(
	struct contest_state_t * const state );

void
contest(
	struct contest_data_t * const bmp,
	const char * const fdir );

int
contest_parse_state(
	struct contest_data_t * const bmp,
	struct contest_state_t * const state );

int
contest_parse_page(
	struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y,
	const signed char vx,
	const signed char vy );

void
contest_search_forward(
	struct contest_data_t * const bmp,
	const char * const fdir );

void
contest_search_backward(
	struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y,
	const signed char vx,
	const signed char vy,
	const signed char clr );

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
