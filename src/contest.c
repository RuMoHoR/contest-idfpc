#include <stdio.h>

#include "contest.h"

struct contest_state_t {
	unsigned int	x;
	unsigned int	y;
	signed char	vx;
	signed char	vy;
	signed char	clr;
};

static
int
contest_parse_step(
	const struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	struct contest_operand_t	*op;

	op = bmpread_get_opcode( bmp, state->x, state->y );

	if ( !op ) {
		return 0;
	} else { /* got opcode */
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
void
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

	if ( !bmp ) return;

	bmpread_alloc_bw( bmp );

	state.x = x;
	state.y = y;
	state.vx = vx;
	state.vy = vy;
	state.clr = 0;

	n = 400000;
	do {
		r = contest_parse_step( bmp, &state );
		n--;
	} while ( r && n );

	bmpread_save( bmp, fdir, fname );
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
