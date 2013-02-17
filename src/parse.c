#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

static
int
contest_parse_step(
	const struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	struct contest_operand_t	*op;

	op = bmpread_get_opcode( bmp, state->x, state->y );

	if ( !op ) {
		contest_dump_stop( state );
		return 0;
	} else { /* got opcode */

		contest_dump( "step", state, op );

		state->vx ^= op->A;
		state->vy ^= op->B;
		state->clr ^= op->C;

		draw_line(
			bmp,
			bmp->result_used,
			state->x, state->y,
			state->x, state->y,
			0xFF );

		if ( ( state->vx > 10) || ( state->vy >16 ) ) {
			state->clr = 0;
		}

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

int
contest_parse_state(
	struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	int	r;
	int n;

	if ( !bmp ) return 0;

	bmpread_alloc_bw( bmp );

	n = 0;
	do {
//		contest_dump_start( &state );
		r = contest_parse_step( bmp, state );
		if ( !state->clr ) {
			contest_dump_stop( state );
		} /* stop dump? */
		n++;
	} while ( r && ( n < 100000 ) );

	if ( n > 50 ) {
		printf( "Steps: %d\n", n );
		state->dump = 1;
		contest_dump( "final", state, NULL );

//		bmpread_save( bmp, fdir, fname );
	}

	return n;
}

int
contest_parse_page(
	struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y,
	const signed char vx,
	const signed char vy )
{
	struct contest_state_t	state;
	int	ret;

	state.x = x;
	state.y = y;
	state.vx = vx;
	state.vy = vy;
	state.clr = 0;
	state.dump = 0;

	ret = contest_parse_state( bmp, &state );

	return ret;
}
