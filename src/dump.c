#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

void
contest_dump(
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

void
contest_dump_start(
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
//		( ( state->x == 24 ) && ( state->y == 71 ) ) ||
//		( ( state->x == 24 ) && ( state->y == 117 ) ) ||
		( ( state->x == 70 ) && ( state->y == 79 ) ) ||
		0
	) {
		state->dump = 1;
		contest_dump( "start", state, NULL );
	}
}

void
contest_dump_stop(
	struct contest_state_t * const state )
{
	contest_dump( "stop", state, NULL );
	state->dump = 0;
}
