#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

static
void
contest_search_forward_letter(
	struct contest_data_t * const bmp,
	const char * const fdir,
	const char fletter,
	const int opa1,
	const int opb1,
	const int opa2,
	const int opb2,
	const int vx,
	const int vy )
{
	char	fname[ PATH_MAX ];
	int	fnum;
	unsigned int	pos;
	int	x, y;

//return;
	fnum = 1;

	for ( pos = 0; (fnum < 1600 ) && ( pos < bmp->count ); pos++ ) {
		if (
			( ( bmp->data[ pos ].A == opa1 ) && ( bmp->data[ pos ].B == opb1 ) ) ||
			( ( bmp->data[ pos ].A == opa2 ) && ( bmp->data[ pos ].B == opb2 ) )
		) {
			x = pos % bmp->width;
			y = bmp->height - 1 - ( pos / bmp->width);
			snprintf(
				fname, PATH_MAX,
				"t%c_%05d_p%06X_%06X_x%03d_y%03d",
				fletter,
				fnum,
				pos * 3, pos * 3 + BMPREAD_HDR_SIZE,
				x, y );
			fnum++;
			contest_parse_page( bmp, fdir, fname, x, y, vx, vy );
		} /* found */
	} /* search all */
}

void
contest_search_forward(
	struct contest_data_t * const bmp,
	const char * const fdir )
{
	contest_search_forward_letter( bmp, fdir, 'A', -2, 4, 9999, 9999, -2, 2 );
	contest_search_forward_letter( bmp, fdir, 'P', 4, -8, 9999, 9999, 0, -8 );
//	contest_search_forward_letter( bmp, fdir, 'R', 6, 2, 9999, 9999, ,  );
}