#include <stdlib.h>
//#include <math.h>
#include "contest.h"

#if 0
static inline
int
abs(
	const int x )
{
	int	ret;

	if ( x < 0 ) {
		ret = -x;
	} else {
		ret = x;
	} /* <0 ? */

	return ret;
}
#endif

static
void
draw_plot_pixel(
	const int xx,
	const int yy )
{
}

#define	DRAW_SCALE	12
void
draw_plot_line(
	const int xxs,
	const int yys,
	const int xxe,
	const int yye )
{
	int	xx0, xx1, yy0, yy1;
	int	x, y, dx, dy, sx, sy, err, e2;

	xx0 = xxs * DRAW_SCALE;
	xx1 = xxe * DRAW_SCALE;
	yy0 = yys * DRAW_SCALE;
	yy1 = yye * DRAW_SCALE;

	dx = abs( xx1 - xx0 );
	sx = ( xx0 < xx1 ) ? 1 : -1;
	dy = -abs( yy1 - yy0 );
	sy = ( yy0 < yy1 ) ? 1 : -1;
	err = dx + dy;		/* error value e_xy */

	x = xx0;
	y = yy0;
	do {  /* loop */
		draw_plot_pixel( x, y );
		e2 = 2*err;
		if ( e2 >= dy ) {
			err += dy;
			x += sx;
		} /* e_xy + e_x > 0 */
		if ( e2 <= dx ) {
			err += dx;
			y += sy;
		} /* e_xy+e_y < 0 */
	} while ( ( x != xx1 ) || ( y != yy1 ) );
}

#if 0
void
rao_draw_plot_box(
	const int xx0,
	const int yy0,
	const int xx1,
	const int yy1 )
{
	int	x, y;

	if ( xx0 < xx1 ) {
		for ( x = xx0; x <= xx1; x++ ) {
			rao_draw_plot_pixel( x, yy0 );
			rao_draw_plot_pixel( x, yy1 );
		} /* horiz line */
	} else { /* reverse */
		for ( x = xx1; x <= xx0; x++ ) {
			rao_draw_plot_pixel( x, yy0 );
			rao_draw_plot_pixel( x, yy1 );
		} /* horiz line */
	} /* x direction */

	if ( yy0 < yy1 ) {
		for ( y = yy0; y <= yy1; y++ ) {
			rao_draw_plot_pixel( xx0, y );
			rao_draw_plot_pixel( xx1, y );
		} /* horiz line */
	} else { /* reverse */
		for ( y = yy1; y <= yy0; y++ ) {
			rao_draw_plot_pixel( xx0, y );
			rao_draw_plot_pixel( xx1, y );
		} /* horiz line */
	} /* x direction */
}

void
rao_draw_plot_circle(
	const int xmid,
	const int ymid,
	const int radius )
{
	int	x, y, err, e2;

	x = -radius;
	y = 0;
	err = 2 - 2 * radius; /* II. Quadrant */

	do {
		rao_draw_plot_pixel( xmid - x, ymid + y ); /*   I. Quadrant */
		rao_draw_plot_pixel( xmid - y, ymid - x ); /*  II. Quadrant */
		rao_draw_plot_pixel( xmid + x, ymid - y ); /* III. Quadrant */
		rao_draw_plot_pixel( xmid + y, ymid + x ); /*  IV. Quadrant */

		e2 = err;

		if ( e2 >  x ) {
			x++;
			err += x * 2 + 1;
		} /* e_xy+e_x > 0 */

		if ( e2 <= y ) {
			y++;
			err += y * 2 + 1;
		} /* e_xy+e_y < 0 */
	} while ( x < 0 );
}
#endif
