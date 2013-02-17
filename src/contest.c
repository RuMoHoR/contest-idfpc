#include <stdio.h>
#include <linux/limits.h>

#include "contest.h"

void
contest(
	struct contest_data_t * const bmp,
	const char * const fdir )
{
	contest_parse_page( bmp, 70, 79, 18, 26 );
	bmpread_save( bmp, fdir, "01_play", bmp->result_bw );
	bmpread_save( bmp, fdir, "91_used", bmp->result_used );

	contest_parse_page( bmp, 50, 22, 24, 34 );
	bmpread_save( bmp, fdir, "02_svg", bmp->result_bw );

	contest_parse_page( bmp, 41, 19, 37, 25 );
	bmpread_save( bmp, fdir, "03_vvvm", bmp->result_bw );

//	contest_parse_page( bmp, fdir, "33_test", 114, 80, -2, 2 );
//	contest_parse_page( bmp, fdir, "34_test", 120, 86, 110, 30 );

	bmpread_save( bmp, fdir, "98_used", bmp->result_used );
	bmpread_save( bmp, fdir, "99_result", bmp->result_color );

	contest_search_forward( bmp, fdir );
	// make bmp|grep final | sort | uniq -c
/*	     28 Dump: [   final] State: x=123 y=183 vx=   0 vy=  -6 clr=120
     18 Dump: [   final] State: x=134 y=158 vx=  -6 vy=   0 clr= 57
      1 Dump: [   final] State: x=270 y= 17 vx=  -6 vy=   0 clr= 71
     22 Dump: [   final] State: x=333 y= 12 vx=   0 vy=   8 clr= 91
     19 Dump: [   final] State: x= 80 y=176 vx=   0 vy=   6 clr=113
*/
}
