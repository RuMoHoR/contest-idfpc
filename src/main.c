#include <stdio.h>

#include "contest.h"

void
contest_error(
	const char *msg )
{
	printf( "Error: %s\n", msg );
}

int
main(
	int argc,
	char *argv[] )
{
	FILE	*fbmp;
	struct contest_data_t	*bmp;

	if ( argc < 2 ) {
		printf("Usage: %s srec\n", argv[ 0 ] );
		return 1;
	} else { /* args ok */
		fbmp = fopen( argv[ 1 ], "r" );
		if ( !fbmp ) {
			printf( "Can't open '%s'!\n", argv[ 1 ] );
			return 2;
		} else { /* open ok */
			bmp = bmpread_read( fbmp );
			fclose( fbmp );
			if ( bmp ) {
				contest_parse( bmp );
			} /* read ok? */
		} /* open ok? */
	} /* args ok? */

	return 0;
}
