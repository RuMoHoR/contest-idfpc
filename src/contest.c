#include <stdio.h>

#include "contest.h"

void
contest(
	struct contest_data_t * const bmp,
	const char * const fdir )
{
/*
	contest_parse_page( bmp, 70, 79, 18, 26 );
	bmpread_save( bmp, fdir, "01_play", bmp->result_bw );
//	bmpread_save( bmp, fdir, "91_used", bmp->result_used );

	contest_parse_page( bmp, 50, 22, 24, 34 );
	bmpread_save( bmp, fdir, "02_svg", bmp->result_bw );

	contest_parse_page( bmp, 41, 19, 37, 25 );
	bmpread_save( bmp, fdir, "03_vvvm", bmp->result_bw );
*/
/*
	bmpread_save( bmp, fdir, "98_used", bmp->result_used );
	bmpread_save( bmp, fdir, "99_result", bmp->result_color );
*/
/*
	contest_search_forward( bmp, fdir );
	// make bmp|grep final | sort | uniq -c
*/
/*
	contest_search_backward( bmp, 134, 158, -6, 0, 57 );
	contest_search_backward( bmp, 80, 176, 0, 6, 113 );
	contest_search_backward( bmp, 333, 12, 0, 8, 91 );
	contest_search_backward( bmp, 123, 183, 0, -6, 120 );
	contest_search_backward( bmp, 270, 17, -6, 0, 71 );
*/
/*
	contest_parse_page( bmp, 97, 35, 36, 43 );
	bmpread_save( bmp, fdir, "11_cmds", bmp->result_bw );

	contest_parse_page( bmp, 106, 15, 18, 0 );
	bmpread_save( bmp, fdir, "12_cartesian", bmp->result_bw );
*/
	contest_vvvm( bmp );
//	contest_produce( fdir );
}
