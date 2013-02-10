#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "contest.h"

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
	int	r;

	data = (struct contest_data_t *)malloc( sizeof( struct contest_data_t ) );

	if ( !data ) {
		contest_error( "Alloc!" );
	} else { /* malloc ok*/
		r = fread( data->hdr, 1, BMPREAD_HDR_SIZE, fbmp );
	printf( "rd = %d\n", r );

		data->width = 400;
		data->height = 200;
		data->size_src =
			data->width *
			data->height *
			sizeof( struct contest_operand_t );
		data->size_res =
			data->width * DRAW_SCALE *
			data->height * DRAW_SCALE *
			sizeof( struct contest_operand_t );

		if ( !( data->size_src ) ) {
			contest_error( "Limit!" );
			data = NULL;
		} else { /* have data */
			data->data = malloc( data->size_src );
			data->result = malloc( data->size_res );
			memset( data->result, 0, data->size_res );
			if ( !( data->data ) ) {
				contest_error( "Data alloc!" );
				free( data );
				data = NULL;
			} else { /* data malloc ok*/
				if ( !( data->result ) ) {
					contest_error( "Result alloc!" );
					free( data->data );
					data->data = NULL;
					free( data );
					data = NULL;
				} else { /* res malloc ok*/
					r = fread( data->data, 1, data->size_src, fbmp );
	printf( "rd = %d\n", r );
				} /* res alloc ok? */
			} /* data alloc ok? */
		} /* have data? */
	} /* malloc ok? */

	return data;
}

void
bmpread_save(
	FILE * const fres,
	const struct contest_data_t * const bmp )
{
	int	r;

	if ( !bmp ) {
	} else { /* malloc ok*/
		r = fwrite( bmp->hdr, 1, BMPREAD_HDR_SIZE, fres );
	printf( "wr = %d\n", r );

		if ( !( bmp->result ) ) {
			contest_error( "No result!" );
		} else { /* res malloc ok*/
			r = fwrite( bmp->result, 1, bmp->size_res, fres );
	printf( "wr = %d\n", r );
		} /* have res? */
	} /* malloc ok? */
}

void
bmpread_free(
	struct contest_data_t * const bmp )
{
	if ( !bmp ) {
		contest_error( "Free NULL!" );
	} else { /* malloc ok*/
		if ( bmp->data ) {
			free( bmp->data );
			bmp->data = NULL;
		} /* have data? */
		if ( bmp->result ) {
			free( bmp->result );
			bmp->result = NULL;
		} /* have res? */
		free( bmp );
	} /* have data? */
}
