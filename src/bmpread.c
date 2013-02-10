#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

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

void
bmpread_alloc_bw(
	struct contest_data_t * const bmp )
{
	if ( bmp->result_bw ) {
		free( bmp->result_bw );
		bmp->result_bw = NULL;
	} /* res alloc ok? */

	bmp->result_bw = malloc( bmp->size_res );

	if ( bmp->result_bw ) {
		memset( bmp->result_bw, 0, bmp->size_res );
	} /* res alloc ok? */
}

void
bmpread_move_res_to_bw(
	struct contest_data_t * const bmp )
{
	if ( ( bmp->result ) && ( bmp->result_bw ) ) {
		memcpy( bmp->result_bw, bmp->result, bmp->size_res );
	} /* may copy? */
}

struct contest_data_t*
bmpread_read(
	FILE * const fbmp )
{
	struct contest_data_t	*bmp;
	int	r;

	bmp = (struct contest_data_t *)malloc( sizeof( struct contest_data_t ) );

	if ( !bmp ) {
		contest_error( "Alloc!" );
	} else { /* malloc ok*/
		r = fread( bmp->hdr, 1, BMPREAD_HDR_SIZE, fbmp );
		printf( "rd = %d\n", r );

		bmp->width = 400;
		bmp->height = 200;
		bmp->size_src =
			bmp->width *
			bmp->height *
			sizeof( struct contest_operand_t );
		bmp->size_res =
			bmp->width * DRAW_SCALE *
			bmp->height * DRAW_SCALE *
			sizeof( struct contest_operand_t );

		if ( !( bmp->size_src ) ) {
			contest_error( "Limit!" );
			bmp = NULL;
		} else { /* have data */
			bmp->data = malloc( bmp->size_src );
			if ( !( bmp->data ) ) {
				contest_error( "Data alloc!" );
				free( bmp );
				bmp = NULL;
			} else { /* data malloc ok*/
				r = fread( bmp->data, 1, bmp->size_src, fbmp );
				printf( "rd = %d\n", r );

				bmp->result = malloc( bmp->size_res );
				if ( bmp->result ) {
					memset( bmp->result, 0, bmp->size_res );
				}
				bmpread_alloc_bw( bmp );
			} /* data alloc ok? */
		} /* have data? */
	} /* malloc ok? */

	return bmp;
}

void
bmpread_save(
	const struct contest_data_t * const bmp,
	const char * const fdir,
	const char * const fname )
{
	char	fpath[ PATH_MAX + 1 ];
	FILE	*fres;
	int	r;

	snprintf(
		fpath,
		PATH_MAX,
		"./%s/%s.bmp",
		fdir,
		fname );


	if ( !bmp ) {
	} else { /* malloc ok*/
		fres = fopen( fpath, "w" );
		if ( !fres ) {
			printf( "Can't open '%s', no result saved!\n", fpath );
		} else { /* open ok */
			r = fwrite( bmp->hdr, 1, BMPREAD_HDR_SIZE, fres );
		printf( "wr = %d\n", r );

			if ( !( bmp->result_bw ) ) {
				contest_error( "No result!" );
			} else { /* res malloc ok*/
				r = fwrite( bmp->result_bw, 1, bmp->size_res, fres );
		printf( "wr = %d\n", r );
			} /* have res? */
		} /* res open ok?*/
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
		if ( bmp->result_bw ) {
			free( bmp->result_bw );
			bmp->result_bw = NULL;
		} /* have res_bw? */
		free( bmp );
	} /* have data? */
}
