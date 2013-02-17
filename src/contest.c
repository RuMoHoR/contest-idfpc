#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

void
contest(
	struct contest_data_t * const bmp,
	const char * const fdir )
{
	contest_parse_page( bmp, fdir, "01_play", 70, 79, 18, 26 );

//	bmpread_fill_bw_from_used( bmp );
//	bmpread_save( bmp, fdir, "91_used" );

	contest_parse_page( bmp, fdir, "02_svg", 50, 22, 24, 34 );
//	contest_parse_page( bmp, fdir, "03_vvvm", 41, 19, 37, 25 );

	contest_parse_page( bmp, fdir, "33_test", 114, 80, -2, 2 );
//	contest_parse_page( bmp, fdir, "34_test", 120, 86, 110, 30 );

//	bmpread_fill_bw_from_used( bmp );
//	bmpread_save( bmp, fdir, "98_used" );

//	bmpread_fill_bw_from_color( bmp );
//	bmpread_save( bmp, fdir, "99_result" );

	contest_search_forward( bmp, fdir );
	// make bmp|grep final | sort | uniq -c
}
