#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

static
void
contest_search_forward_letter(
	struct contest_data_t * const bmp,
	const int opa1,
	const int opb1,
	const int opa2,
	const int opb2,
	const int vx,
	const int vy )
{
	unsigned int	pos;
	int	num;
	int	x, y;

//return;
	num = 1;

	for ( pos = 0; (num < 1600 ) && ( pos < bmp->count ); pos++ ) {
		if (
			( ( bmp->data[ pos ].A == opa1 ) && ( bmp->data[ pos ].B == opb1 ) ) ||
			( ( bmp->data[ pos ].A == opa2 ) && ( bmp->data[ pos ].B == opb2 ) )
		) {
			x = pos % bmp->width;
			y = bmp->height - 1 - ( pos / bmp->width);
			contest_parse_page( bmp, x, y, vx, vy );
			num++;
		} /* found */
	} /* search all */
}

void
contest_search_forward(
	struct contest_data_t * const bmp,
	const char * const fdir )
{
	contest_search_forward_letter( bmp, -2, 4, 9999, 9999, -2, 2 ); // A
	contest_search_forward_letter( bmp, 4, -8, 9999, 9999, 0, -8 ); // P
//	contest_search_forward_letter( bmp, fdir, 'R', 6, 2, 9999, 9999, ,  );
}

static
int
contest_search_backward_step(
	struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	struct contest_operand_t	*op;

	state->x = state->x - state->vx;
	state->y = state->y - state->vy;

	op = bmpread_get_opcode( bmp, state->x, state->y );

//	contest_dump( "bkstep", state, op );

	if ( !op ) {
		return 0;
	} else { /* got opcode */

		state->vx ^= op->A;
		state->vy ^= op->B;
		state->clr ^= op->C;
//		contest_dump( "bkafter", state, op );
		return 1;
	} /* opcode ok */
}
//#if 0
static
int
contest_search_backward_check(
	struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	struct contest_operand_t	*op;

	op = bmpread_get_opcode( bmp, state->x, state->y );

//	contest_dump( "ckstep", state, op );

	if ( !op ) {
		return 0;
	} else { /* got opcode */

		state->vx ^= op->A;
		state->vy ^= op->B;
		state->clr ^= op->C;
	state->x = state->x + state->vx;
	state->y = state->y + state->vy;

//	contest_dump( "ckafter", state, op );
		return 1;
	} /* opcode ok */
}
//#endif

static
void
contest_search_backward_state(
	struct contest_data_t * const bmp,
	struct contest_state_t * const state )
{
	struct contest_state_t	pre;
	struct contest_state_t	post;
	struct contest_state_t	start;
	int	r, c;
	int n;

	if ( !bmp ) return;

	bmpread_alloc_bw( bmp );

//	contest_dump( "back", state, NULL );
	n = 0;
	start = *state;
	do {
//		contest_dump( "--", state, NULL );
		pre = *state;
		r = contest_search_backward_step( bmp, state );
		post = *state;
		c = contest_search_backward_check( bmp, &post );
		if ( !state->clr && r && c) {
			start = *state;
		} /* start point?*/
//		printf("\n");
		n++;
	} while ( r && c && ( n < 100000 ) );

	contest_dump( "zero", &start, NULL );

	/*
	if ( n > 50 ) {
		printf( "Steps: %d\n", n );
		state->dump = 1;
		contest_dump( "final", state, NULL );

//		bmpread_save( bmp, fdir, fname );
	}
*/
}
/*
Dump: [  prefin] State: x= 24 y=  4 vx=   0 vy=  -8 clr= 56
Dump: [  prefin] State: x= 30 y=  4 vx=   6 vy=   0 clr= 44
Dump: [  prefin] State: x=134 y=127 vx= 104 vy= 123 clr=  0
Dump: [  prefin] State: x=140 y=158 vx=   6 vy=  31 clr=  0
Dump: [  prefin] State: x=134 y=158 vx=  -6 vy=   0 clr= 57
Dump: [  prefin] State: x=134 y=158 vx=  -6 vy=   0 clr= 57
*/
void
contest_search_backward(
	struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y,
	const signed char vx,
	const signed char vy,
	const signed char clr )
{
	struct contest_state_t	state;

	state.x = x;
	state.y = y;
	state.vx = vx;
	state.vy = vy;
	state.clr = clr;
	state.dump = 1;

	contest_search_backward_state( bmp, &state );
}
