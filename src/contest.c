#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

struct contest_state_t {
	unsigned int	x;
	unsigned int	y;
	signed char	vx;
	signed char	vy;
	signed char	clr;
	int	dump;
};

static
void
contest_parse_dump(
	const char * const label,
	const struct contest_state_t * const state,
	const struct contest_operand_t * const op )
{
	if ( state->dump ) {

		printf( "Dump: [%8s] State: x=%3u y=%3u vx=%4d vy=%4d clr=%3u",
			label,
			state->x, state->y,
			state->vx, state->vy,
			state->clr );
		if ( op ) {
			printf(
				" Op: A=%4d B=%4d C=%4d",
				op->A, op->B, op->C );
		} /* have op? */
		printf( "\n" );
	} /* do dump? */
}

static
void
contest_parse_dump_start(
	struct contest_state_t * const state )
{
	if (
//		( ( state->x == 180 ) && ( state->y == 56 ) ) ||
//		( ( state->x == 210 ) && ( state->y == 56 ) ) ||
//		( ( state->x == 120 ) && ( state->y == 86 ) ) ||
//		( ( state->x == 114 ) && ( state->y == 80 ) ) ||
//		( ( state->x == 20 ) && ( state->y == 87 ) ) ||
//		( ( state->x == 70 ) && ( state->y == 117 ) ) ||

//		( ( state->x == 174 ) && ( state->y == 101 ) ) ||
//		( ( state->x == 84 ) && ( state->y == 117 ) ) ||
		( ( state->x == 24 ) && ( state->y == 71 ) ) ||
		( ( state->x == 24 ) && ( state->y == 117 ) ) ||
		0
	) {
		state->dump = 1;
		contest_parse_dump( "start", state, NULL );
	}
}

static
void
contest_parse_dump_stop(
	struct contest_state_t * const state )
{
	contest_parse_dump( "start", state, NULL );
	state->dump = 0;
}

static
int
contest_parse_step(
	const struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	struct contest_operand_t	*op;

	op = bmpread_get_opcode( bmp, state->x, state->y );

	if ( !op ) {
		contest_parse_dump_stop( state );
		return 0;
	} else { /* got opcode */

		contest_parse_dump( "step", state, op );

		state->vx ^= op->A;
		state->vy ^= op->B;
		state->clr ^= op->C;

		draw_line(
			bmp,
			bmp->result_used,
			state->x, state->y,
			state->x, state->y,
			0xFF );

		if ( !state->clr ) {
		} else { /* draw */
			draw_line(
				bmp,
				bmp->result_color,
				state->x, state->y,
				state->x + state->vx,
				state->y + state->vy,
				state->clr );
			draw_line(
				bmp,
				bmp->result_bw,
				state->x, state->y,
				state->x + state->vx,
				state->y + state->vy,
				0xFF );
		} /* color ok? */

		state->x = state->x + state->vx;
		state->y = state->y + state->vy;
		return 1;
	} /* opcode ok */
}

static
int
contest_parse_page(
	struct contest_data_t * const bmp,
	const char * const fdir,
	const char * const fname,
	const unsigned int x,
	const unsigned int y,
	const signed char vx,
	const signed char vy )
{
	struct contest_state_t	state;
	int	r;
	int n;

	if ( !bmp ) return 0;

	printf( "Making \"%s\"\n", fname );
	bmpread_alloc_bw( bmp );

	state.x = x;
	state.y = y;
	state.vx = vx;
	state.vy = vy;
	state.clr = 0;
	state.dump = 0;

	n = 0;
	do {
		contest_parse_dump_start( &state );
		r = contest_parse_step( bmp, &state );
		if ( !state.clr ) {
			contest_parse_dump_stop( &state );
		} /* stop dump? */
		n++;
	} while ( r && ( n < 100000 ) );

	if ( n > 20 ) {
		bmpread_save( bmp, fdir, fname );
	}

	return n;
}

void
contest_parse(
	struct contest_data_t * const bmp,
	const char * const fdir )
{
	contest_parse_page( bmp, fdir, "01_play", 70, 79, 18, 26 );

	bmpread_fill_bw_from_used( bmp );
	bmpread_save( bmp, fdir, "91_used" );

	contest_parse_page( bmp, fdir, "02_svg", 50, 22, 24, 34 );
	contest_parse_page( bmp, fdir, "03_vvvm", 41, 19, 37, 25 );

	bmpread_fill_bw_from_used( bmp );
	bmpread_save( bmp, fdir, "98_used" );

	bmpread_fill_bw_from_color( bmp );
	bmpread_save( bmp, fdir, "99_result" );
}
