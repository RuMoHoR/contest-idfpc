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

//	printf( "\n" );

	printf(
		"ParseStep: x=%3u y=%3u vx=%4d vy=%4d  ",
		state->x, state->y,
		state->vx, state->vy );

	op = bmpread_get_opcode( bmp, state->x, state->y );

	if ( !op ) {
		printf( "\n" );
		return 0;
	} else { /* got opcode */
//		printf(
//			"ParseStep: A=%d B=%d C=%d X=d\n",
//				op->A, op->B, op->C/*, op->X*/ );

		state->vx ^= op->A;
		state->vy ^= op->B;
		state->clr ^= op->C;

		if ( !state->clr ) {
			printf(
//				"ParseStep: skip [%3u,%3u]-[%3u,%3u]\n",
				"skip [%3u,%3u]-[%3u,%3u]\n",
				state->x, state->y,
				state->x + state->vx,
				state->y + state->vy );
		} else { /* draw */
			printf(
//				"ParseStep: line [%3u,%3u]-[%3u,%3u] c=%d\n",
				"line [%3u,%3u]-[%3u,%3u] c=%d\n",
				state->x, state->y,
				state->x + state->vx,
				state->y + state->vy,
				state->clr );
			draw_line(
				bmp,
				state->x, state->y,
				state->x + state->vx,
				state->y + state->vy,
				state->clr );
		} /* color ok? */

		state->x = state->x + state->vx;
		state->y = state->y + state->vy;

//		printf(
//			"ParseStep: new x=%u y=%u vx=%d vy=%d\n",
//			state->x, state->y,
//			state->vx, state->vy );

		return 1;
	} /* opcode ok */
}

void
contest_parse(
	const struct contest_data_t * const bmp )
{
	struct contest_state_t	state;
	int	r;
	int n;

	if ( !bmp ) return;

	state.x = 70;
	state.y = 79;
	state.vx = 18;
	state.vy = 26;
	state.clr = 0;

	n = 400000;
//	n = 8;
	do {
		r = contest_parse_step( bmp, &state );
		n--;
	} while ( r && n );
}
