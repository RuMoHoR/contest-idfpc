#include <stdio.h>

#include "contest.h"

struct contest_state_t {
	unsigned int	x;
	unsigned int	y;
	signed char	vx;
	signed char	vy;
};

static
int
contest_parse_step(
	const struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	struct contest_operand_t	*op;

//	printf( "\n" );

//	printf(
//		"ParseStep: x=%u y=%u vx=%d vy=%d\n",
//		state->x, state->y,
//		state->vx, state->vy );

	op = bmpread_get_opcode( bmp, state->x, state->y );

	if ( !op ) {
		return 0;
	} else { /* got opcode */
//		printf(
//			"ParseStep: A=%d B=%d C=%d X=d\n",
//				op->A, op->B, op->C/*, op->X*/ );

		state->vx ^= op->A;
		state->vy ^= op->B;

		printf(
			"ParseStep: line x= [%u - %u] y= [%u - %u]\n",
			state->x, state->x + state->vx,
			state->y, state->y + state->vy );

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

	if ( !bmp ) return;

	printf(
		"sz=%zu %zu %zu\nw=%d h=%d\n",
		sizeof( bmp ), sizeof( *bmp ), sizeof( struct contest_operand_t ),
		bmp->width, bmp->height );

	state.x = 70;
	state.y = 79;
	state.vx = 18;
	state.vy = 26;

	do {
		r = contest_parse_step( bmp, &state );
	} while ( r );
}
