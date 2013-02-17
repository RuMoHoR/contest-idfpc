#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

void
contest_search_forward(
	struct contest_data_t * const bmp,
	const char * const fdir )
{
	char	fname[ PATH_MAX ];
	int	fnum;
	unsigned int	pos;
	int	x, y;

//return;
	fnum = 1;

	for ( pos = 0; (fnum < 1600 ) && ( pos < bmp->count ); pos++ ) {
		if (
//			( ( bmp->data[ pos ].A == -2 ) && ( bmp->data[ pos ].B == 4 ) ) ||	// A -2.2
			( ( bmp->data[ pos ].A == 4 ) && ( bmp->data[ pos ].B == -8 ) ) ||	// P 4.0
//			( ( bmp->data[ pos ].A == 6 ) && ( bmp->data[ pos ].B == 2 ) ) ||	// R
			0
		) {
			x = pos % bmp->width;
			y = bmp->height - 1 - ( pos / bmp->width);
			snprintf(
				fname, PATH_MAX,
				"t_%05d_p%06X_%06X_x%03d_y%03d",
				fnum,
				pos * 3, pos * 3 + BMPREAD_HDR_SIZE,
				x, y );
			fnum++;
			contest_parse_page( bmp, fdir, fname, x, y, 0, -8 );
		} /* found */
	} /* search all */
}
