#include <stdlib.h>
#include <stdio.h>

#include "contest.h"

#define	BMPREAD_HDR_SIZE	54

struct contest_operand_t*
bmpread_get_opcode(
	const struct contest_data_t * const bmp,
	const unsigned int x,
	const unsigned int y )
{
	struct contest_operand_t	*ret;
	unsigned int	pos;

//	printf(
//		"GetOp: bmp=%p x=%u y=%u\n",
//		bmp, x, y );
//	printf(
//		"GetOp: x=%u y=%u\n",
//		x, y );

	if ( !bmp ) return NULL;

	if (
		( x >= bmp->width ) ||
		( y >= bmp->height )
	) { /* bad coords*/
		return NULL;
	} else { /* good coords */
		pos = ( bmp->height - y - 1 ) * bmp->width + x;
		ret = &( bmp->data[ pos ] );

//		printf(
//			"GetOp: p0=%p p1=%p s=%zu\n",
//			&( bmp->data[ 0 ] ), &( bmp->data[ 1 ] ), sizeof( bmp->data[ 0 ] ) );
//		printf(
//			"GetOp: pos=%u ret=%p A=%d B=%d C=%d X=%d\n",
//			pos, ret, ret->A, ret->B, ret->C, ret->X );
//		printf(
//			"GetOp: pos=%u/%u A=%d B=%d C=%d X=d\n",
//			pos, pos*3,ret->A, ret->B, ret->C/*, ret->X */);

		if (
			!( ret->A ) &&
			!( ret->B ) &&
			!( ret->C )
		) {
			return NULL;
		} else { /* good opcode */
			return ret;
		} /* halt opcode? */
	} /* coord ok? */
}

struct contest_data_t*
bmpread_read(
	FILE * const fbmp )
{
	struct contest_data_t	*data;
	unsigned char	hdr[ BMPREAD_HDR_SIZE ];
	int	r;

	r = fread( hdr, 1, BMPREAD_HDR_SIZE, fbmp );
	printf( "rd = %d\n", r );

	data = (struct contest_data_t *)malloc( sizeof( struct contest_data_t ) );

	if ( !data ) {
		contest_error( "Alloc!" );
	} else { /* malloc ok*/
		data->width = 400;
		data->height = 200;
		data->limit = data->width * data->height;
		if ( !( data->limit ) ) {
			contest_error( "Limit!" );
			data = NULL;
		} else { /* have data */
			data->data = malloc( data->limit * sizeof( struct contest_operand_t ) );
			if ( !( data->data ) ) {
				contest_error( "Data alloc!" );
				free( data );
				data = NULL;
			} else { /* data malloc ok*/
				r = fread( data->data, 1, data->limit * sizeof( struct contest_operand_t ), fbmp );
	printf( "rd = %d\n", r );
			} /* ,alloc ok? */
		} /* have data? */
	} /* malloc ok? */

	return data;
}
