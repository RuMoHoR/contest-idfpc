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

static
void
bmpread_free_operands(
	struct contest_operand_t *  *  const data )
{
	if ( *data ) {
		free( *data );
		*data = NULL;
	} /* have data? */
}

void
bmpread_free(
	struct contest_data_t * const bmp )
{
	if ( !bmp ) {
		contest_error( "Free NULL!" );
	} else { /* malloc ok*/
		bmpread_free_operands( &( bmp->data ) );
		bmpread_free_operands( &( bmp->result_color ) );
		bmpread_free_operands( &( bmp->result_bw ) );
		bmpread_free_operands( &( bmp->result_used ) );
		free( bmp );
	} /* have data? */
}

void
bmpread_alloc(
	struct contest_operand_t *  *  const data,
	const size_t size )
{
	*data = malloc( size );

	if ( *data ) {
		memset( *data, 0x00, size );
	} /* alloc ok? */
}

void
bmpread_alloc_bw(
	struct contest_data_t * const bmp )
{
	bmpread_free_operands( &( bmp->result_bw ) );
	bmpread_alloc( &( bmp->result_bw ), bmp->size );
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

		bmp->width = 400;
		bmp->height = 200;
		bmp->count =
			bmp->width *
			bmp->height;
		bmp->size =
			bmp->count *
			sizeof( struct contest_operand_t );

		if ( !( bmp->size ) ) {
			contest_error( "Limit!" );
			bmp = NULL;
		} else { /* have data */
			bmp->data = malloc( bmp->size );
			if ( !( bmp->data ) ) {
				contest_error( "Data alloc!" );
				free( bmp );
				bmp = NULL;
			} else { /* data malloc ok*/
				r = fread( bmp->data, 1, bmp->size, fbmp );
				bmpread_alloc( &( bmp->result_color ), bmp->size );
				bmpread_alloc( &( bmp->result_bw ), bmp->size );
				bmpread_alloc( &( bmp->result_used ), bmp->size );
			} /* data alloc ok? */
		} /* have data? */
	} /* malloc ok? */

	return bmp;
}

void
bmpread_save(
	const struct contest_data_t * const bmp,
	const char * const fdir,
	const char * const fname,
	const struct contest_operand_t *result )
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
			printf( "Saving '%s'.\n", fpath );
			r = fwrite( bmp->hdr, 1, BMPREAD_HDR_SIZE, fres );
//			printf( "wr = %d\n", r );

			if ( !result ) {
				contest_error( "No result!" );
			} else { /* res malloc ok*/
				r = fwrite( result, 1, bmp->size, fres );
//				printf( "wr = %d\n", r );
			} /* have res? */
		} /* res open ok?*/
	} /* malloc ok? */
}
