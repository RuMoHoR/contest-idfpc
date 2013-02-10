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
	FILE	*fres;
	struct contest_data_t	*bmp;

	printf("Usage: %d\n", argc );

	if ( argc < 2 ) {
		printf("Usage: %s pic [res]\n", argv[ 0 ] );
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
				if ( argc >= 3 ) {
					fres = fopen( argv[ 2 ], "w" );
					if ( !fres ) {
						printf( "Can't open '%s', no result saved!\n", argv[ 2 ] );
					} else { /* open ok */
						bmpread_save( fres, bmp );
					} /* res open ok?*/
				} /* have res name? */
				bmpread_free( bmp );
			} /* read ok? */
		} /* open ok? */
	} /* args ok? */

	return 0;
}
