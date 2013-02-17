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