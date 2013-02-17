#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

#include "contest.h"

#define	PRODUCER_SIZE	1048576

struct contest_producer_t {
	int	value;
	int	next;
	int	A;
	int	B;
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
			prod[ i ].value = 0;
			prod[ i ].next = rand() % 256;
			prod[ i ].A = 0;
			prod[ i ].B = 0;
			prod[ i ].parent_num = 0;
			prod[ i ].parent[ 0 ] = -1;
			prod[ i ].parent[ 1 ] = -1;
			prod[ i ].parent[ 2 ] = -1;
			prod[ i ].parent[ 3 ] = -1;
		} /* all items */
	} /* alloc ok? */
	return prod;
}

#define	PRODUCER_LINE_SIZE	160

static
int
produce_data_read_line_pid(
	struct contest_producer_t * const prod,
	const char * const line )
{
	int	n;
	int	pid;
	int	ret;

	ret = 0;
	n = sscanf( line, "Process %d\n", &pid );
	if ( n > 0 ) {
		printf( "PID: %d [%d]\n", n, pid );
		ret = 1;
	} /* pid? */
	return ret;
}

static
int
produce_data_read_line_send(
	struct contest_producer_t * const prod,
	const char * const line )
{
	int	n;
	int	pid;
	int	ret;

	ret = 0;
	n = sscanf( line, "  send Value to process %d,\n", &pid );
	if ( n > 0 ) {
		printf( "SEND: %d [%d]\n", n, pid );
		ret = 1;
	} /* pid? */
	return ret;
}

static
int
produce_data_read_line_value1(
	struct contest_producer_t * const prod,
	const char * const line )
{
	int	n;
	int	vala, valb;
	char	c1;
	int	ret;

	ret = 0;
	n = sscanf( line, "  Value <- %d * %c / 64 +%d\n", &vala, &c1, &valb );
	if ( n == 3 ) {
		printf( "VAL: %d [%d][%d]\n", n, vala, valb );
		ret = 1;
	} /* pid? */
	return ret;
}

static
int
produce_data_read_line_value2(
	struct contest_producer_t * const prod,
	const char * const line )
{
	int	n;
	int	vala, valb;
	char	c1;
	int	ret;

	ret = 0;
	n = sscanf( line, "  Value <- %d * %c / 64 %d\n", &vala, &c1, &valb );
	if ( n == 3 ) {
		printf( "VAL: %d [%d][%d]\n", n, vala, valb );
		ret = 1;
	} /* pid? */
	return ret;
}

static
int
produce_data_read_line(
	struct contest_producer_t * const prod,
	const char * const line )
{
	int	ret;
	int	r;

	ret = 0;

//	printf( "Line: [%s]\n", line );

	r = produce_data_read_line_pid( prod, line );
	if ( r ) {
		ret = 1;
	} /* pid?*/

	r = produce_data_read_line_send( prod, line );
	if ( r ) {
		ret = 1;
	} /* pid?*/

	r = produce_data_read_line_value1( prod, line );
	if ( r ) {
		ret = 1;
	} /* pid?*/

	r = produce_data_read_line_value2( prod, line );
	if ( r ) {
		ret = 1;
	} /* pid?*/

	if ( !ret ) {
//		printf( "Line: [%s]\n", line );
	}

	ret = 1;

	return ret;
}

static
int
produce_data_read(
	struct contest_producer_t * const prod,
	const char * const fname )
{
	FILE	*falgo;
	char	line[ PRODUCER_LINE_SIZE + 1 ];
	int	lines;
	char	*r;
	int	p;
	int	ret;

	ret = 0;
	falgo = fopen( fname, "r" );
	if ( !falgo ) {
		printf( "Can't open '%s', no algo!\n", fname );
		ret = 0;
	} else { /* open ok */
		lines = 0;
		do {
			r = fgets( line, PRODUCER_LINE_SIZE, falgo );
			if ( r ) {
				p = produce_data_read_line( prod, line );
				lines++;
			} /* have line? */
		} while ( r && p && ( lines < 28 ) );
		ret = 1;
	} /* open oK? */

	return ret;
}

static
int
produce_data_validate(
	struct contest_producer_t * const prod )
{
	int	i;
	int	parent;
	int	ret;
	int	errors;

	ret = 1;
	errors = 0;
	for ( i = 0; ( i < PRODUCER_SIZE) && ( errors < 3 ); i++ ) {
		if ( prod[ i ].parent_num != 4 ) {
			printf( "CP: num_parent = %d\n", prod[ i ].parent_num );
			ret = 0;
			errors++;
		} /* bad num parent */
		for ( parent = 0; parent < 4; parent++ ) {
			if ( prod[ i ].parent[ parent ] == -1 ) {
				printf( "CP: parent[%d] missing\n", parent );
				ret = 0;
				errors++;
			} /* bad parent */
		} /* all parents */
	} /* all items */
	return ret;
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
		color = prod[ i ].next;
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
	int	r;

	prod = produce_data_init();

	if ( prod ) {

		produce_data_read( prod, "algo" );
		r = produce_data_validate( prod );
		if ( r ) {
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
		} /* check ok? */
	} /* alloc ok? */
}
