#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "contest.h"

#define	VVVM_SIZE	13371111

union vvvm_opcode_t {
	struct {
		unsigned char A;
		unsigned char B;
		unsigned char C;
		unsigned char D;
	} bytes;
	unsigned int	word;
};

struct vvvm_t {
	union vvvm_opcode_t	*data;
	size_t	mem;
	unsigned int	ip;
};

static
void
vvvm_run(
	struct vvvm_t * const vm )
{
	printf( "IP=%u C=%08X\n",
		vm->ip,
		vm->data[ vm->ip ].word );
}

void
contest_vvvm(
	const struct contest_data_t * const bmp )
{
	struct vvvm_t	vm;

	vm.ip = 32;
	vm.mem = VVVM_SIZE * sizeof( union vvvm_opcode_t );
	vm.data = malloc( vm.mem );
	if ( vm.data ) {
		memset( vm.data, 0x00, vm.mem );
		if ( bmp->size <= vm.mem ) {
			memcpy( vm.data, bmp->data, bmp->size );
			vvvm_run( &vm );
		} /* size ok? */
		free( vm.data );
	} /* malloc ok? */
}
