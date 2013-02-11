#include <stdlib.h>
#include "contest.h"

static
void
draw_pixel(
	const struct contest_data_t * const bmp,
	struct contest_operand_t * const pixels,
	const unsigned int xx,
	const unsigned int yy,
	const signed char color,
	const signed char color2 )
{
	struct contest_operand_t	*pixel;
	unsigned int	pos;

	if ( !bmp ) return;
	if ( !pixels ) return;

	if (
		( xx >= bmp->width ) ||
		( yy >= bmp->height )
	) { /* bad coords*/
	} else { /* good coords */
		pos = ( bmp->height - yy - 1 ) * bmp->width + xx;

		pixel = &( pixels[ pos ] );
		pixel->A = color;
		pixel->B = color2;
		pixel->C = color2;
	} /* have result? */
}

void
draw_line(
	const struct contest_data_t * const bmp,
	struct contest_operand_t * const pixels,
	const int xxs,
	const int yys,
	const int xxe,
	const int yye,
	const signed char color )
{
	int	xx0, xx1, yy0, yy1;
	int	x, y, dx, dy, sx, sy, err, e2;

	if ( !bmp ) return;
	if ( !pixels ) return;

	xx0 = xxs * DRAW_SCALE;
	xx1 = xxe * DRAW_SCALE;
	yy0 = yys * DRAW_SCALE;
	yy1 = yye * DRAW_SCALE;

	if ( ( xx0 == xx1 ) && ( yy0 == yy1 ) ) {
	} else {
		dx = abs( xx1 - xx0 );
		sx = ( xx0 < xx1 ) ? 1 : -1;
		dy = -abs( yy1 - yy0 );
		sy = ( yy0 < yy1 ) ? 1 : -1;
		err = dx + dy;		/* error value e_xy */

		x = xx0;
		y = yy0;
		do {  /* loop */
			draw_pixel( bmp, pixels, x, y, color, 0x70 );
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
	} /* one pixel? */

	draw_pixel( bmp, pixels, xx0, yy0, color, 0x90 );
	draw_pixel( bmp, pixels, xx1, yy1, color, 0xC0 );
}
