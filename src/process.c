#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "contest.h"

static
unsigned char
contest_produce_create_hdr[ BMPREAD_HDR_SIZE ] = {
0x42, 0x4D, 0x36, 0x00, 0x10, 0x00, 0x00, 0x00,
0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04,
0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static
struct contest_data_t*
contest_produce_create( void )
{
	struct contest_data_t	*bmp;

	bmp = (struct contest_data_t *)malloc( sizeof( struct contest_data_t ) );

	if ( !bmp ) {
		contest_error( "Alloc2!" );
	} else { /* malloc ok*/
//		r = fread( bmp->hdr, 1, BMPREAD_HDR_SIZE, fbmp );
		memcpy( bmp->hdr, &( contest_produce_create_hdr[ 0 ] ), BMPREAD_HDR_SIZE );

		bmp->width = 1024;
		bmp->height = 1024;
		bmp->count =
			bmp->width *
			bmp->height;
		bmp->size =
			bmp->count *
			sizeof( struct contest_operand_t );

		if ( !( bmp->size ) ) {
			contest_error( "Limit2!" );
			bmp = NULL;
		} else { /* have data */
			bmpread_alloc( &( bmp->result_color ), bmp->size );
		} /* have data? */
	} /* malloc ok? */

	printf("%p\n", bmp->result_color );
	return bmp;
}

void
contest_produce(
	const char * const fdir )
{
	struct contest_data_t	*bmp;

	bmp = contest_produce_create();

	bmpread_save( bmp, fdir, "50_initial", bmp->result_color );

	bmpread_free( bmp );
}

