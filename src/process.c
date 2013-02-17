#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

#include "contest.h"

#define	PRODUCER_SIZE	1048576

struct contest_producer_t {
	int	value;
	int	next;
	int	parent[ 4 ];
	int	parent_num;
};

static
unsigned char
produce_image_create_hdr[ BMPREAD_HDR_SIZE ] = {
0x42, 0x4D, 0x36, 0x00, 0x10, 0x00, 0x00, 0x00,
0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04,
0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static
struct contest_data_t*
produce_image_create( void )
{
	struct contest_data_t	*bmp;

	bmp = (struct contest_data_t *)malloc( sizeof( struct contest_data_t ) );

	if ( !bmp ) {
		contest_error( "Alloc2!" );
	} else { /* malloc ok*/
//		r = fread( bmp->hdr, 1, BMPREAD_HDR_SIZE, fbmp );
		memcpy( bmp->hdr, &( produce_image_create_hdr[ 0 ] ), BMPREAD_HDR_SIZE );

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

	return bmp;
}

static
struct contest_producer_t*
produce_data_init( void )
{
	struct contest_producer_t	*prod;
	int	i;

	prod = malloc( PRODUCER_SIZE * sizeof( struct contest_producer_t ) );
	if ( prod ) {
		for ( i = 0; i < PRODUCER_SIZE; i++ ) {
			prod[ i ].value = i *24;
			prod[ i ].next = ( i * 73 ) % 256;
		} /* all items */
	} /* alloc ok? */
	return prod;
}

static
void
produce_copy(
	struct contest_producer_t * const prod )
{
	int	i;

	for ( i = 0; i < PRODUCER_SIZE; i++ ) {
		prod[ i ].value = prod[ i ].next;
	} /* all items */
}

static
void
produce_calculate(
	struct contest_producer_t * const prod )
{
	int	i;

	for ( i = 0; i < PRODUCER_SIZE; i++ ) {
		prod[ i ].next = prod[  i ^ 1 ].value * 133 +55 +i*7;
	} /* all items */
}

static
void
produce_image_fill(
	struct contest_producer_t * const prod,
	struct contest_data_t * const bmp )
{
	int	i;
	int	color;

	for ( i = 0; i < PRODUCER_SIZE; i++ ) {
		color = prod[ i ].value;
		bmp->result_color[ i ].A = color;
		bmp->result_color[ i ].B = color;
		bmp->result_color[ i ].C = color;
	} /* all items */
}

void
contest_produce(
	const char * const fdir )
{
	struct contest_producer_t	*prod;
	struct contest_data_t	*bmp;
	char	fname[ PATH_MAX + 1 ];
	int	step;

	prod = produce_data_init();

	if ( prod ) {
		bmp = produce_image_create();
		produce_image_fill( prod, bmp );
		bmpread_save( bmp, fdir, "50_initial", bmp->result_color );

		for ( step = 1; step <= 7; step++ ) {
			produce_copy( prod );
			produce_calculate( prod );
			produce_image_fill( prod, bmp );
			snprintf(
				fname, PATH_MAX,
				"5%d_step%d",
				step, step );
			bmpread_save( bmp, fdir, fname, bmp->result_color );
		} /* 7 steps */

		bmpread_free( bmp );
	} /* alloc ok? */
}
