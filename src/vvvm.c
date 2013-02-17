#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "contest.h"

#define	VVVM_SIZE	13371111

union vvvm_opcode_t {
	struct {
		signed short offset;
		unsigned short code;
	} cmd;
	unsigned int	word;
};

struct vvvm_t {
	union vvvm_opcode_t	*data;
	size_t	mem;
	unsigned int	ip;
};

static
int
vvvm_get(
	struct vvvm_t * const vm,
	const unsigned int offset,
	union vvvm_opcode_t * const op )
{
	int	ret;

	if ( ( vm->ip + offset ) >= VVVM_SIZE ) {
		printf( "Bad IP+Offs=%u+%u=%u\n",
			vm->ip, offset, vm->ip + offset );
		op->word = 0;
		ret = 0;
	} else { /* addr ok */
		*op = vm->data[ vm->ip + offset ];
		ret = 1;
	} /* addr ok? */

	return ret;
}

static
int
vvvm_binop_plus(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	*a = b + c;
	return 1;
}

static
int
vvvm_binop_minus(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	*a = b - c;
	return 1;
}

static
int
vvvm_binop_mul(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	*a = b * c;
	return 1;
}

static
int
vvvm_binop_div(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	if ( !c ) return 0;

	*a = b / c;
	return 1;
}

static
int
vvvm_binop_and(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	*a = b & c;
	return 1;
}

static
int
vvvm_binop_or(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	*a = b | c;
	return 1;
}

static
int
vvvm_binop_shl(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	*a = b << c;
	return 1;
}

static
int
vvvm_binop_shr(
	unsigned int *a,
	const unsigned int b,
	const unsigned int c )
{
	*a = b >> c;
	return 1;
}

static
int
vvvm_cmd_binop(
	struct vvvm_t * const vm,
	const unsigned int ip,
	int (*func)(
		unsigned int *a,
		const unsigned int b,
		const unsigned int c ) )
{
	unsigned int	aa;
	int	ret;

	ret = func( &aa, 1, 2 );

	return ret;
}

static
int
vvvm_cmd_op_not(
	struct vvvm_t * const vm,
	const unsigned int ip )
{
	int	ret;

	ret =1;

	return ret;
}

static
int
vvvm_cmd_op_jmp(
	struct vvvm_t * const vm,
	const unsigned int ip )
{
	int	ret;

	ret =1;

	return ret;
}

static
int
vvvm_cmd_op_print(
	struct vvvm_t * const vm,
	const unsigned int ip )
{
	int	ret;

	ret =1;

	return ret;
}

static
int
vvvm_cmd(
	struct vvvm_t * const vm,
	const unsigned int ip,
	const unsigned short cmd	)
{
	int	ret;

	ret = 1;
	switch ( cmd ) {
	case 1:
		printf( "CMD: +\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_plus );
		break;
	case 2:
		printf( "CMD: -\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_minus );
		break;
	case 3:
		printf( "CMD: *\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_mul );
		break;
	case 4:
		printf( "CMD: /\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_div );
		break;
	case 5:
		printf( "CMD: &\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_and );
		break;
	case 6:
		printf( "CMD: |\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_or );
		break;
	case 7:
		printf( "CMD: <\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_shl );
		break;
	case 8:
		printf( "CMD: >\n" );
		ret = vvvm_cmd_binop( vm, ip, vvvm_binop_shr );
		break;
	case 9:
		printf( "CMD: ~\n" );
		ret = vvvm_cmd_op_not( vm, ip );
		break;
	case 10:
		printf( "CMD: J\n" );
		ret = vvvm_cmd_op_jmp( vm, ip );
		break;
	case 11:
		printf( "CMD: P\n" );
		ret = vvvm_cmd_op_print( vm, ip );
		break;
	default:
		ret = 0;
		break;
	} /* sw cmd */

	return ret;
}

static
int
vvvm_step(
	struct vvvm_t * const vm )
{
	union vvvm_opcode_t	op;
	int	ret;
	int	r;

	ret = 0;
	r = vvvm_get( vm, 0, &op );
	if ( r ) {
		printf( "IP=%u C=%08X cmd=%04X offs=%d\n",
			vm->ip, op.word, op.cmd.code, op.cmd.offset );
		ret = vvvm_cmd( vm, vm->ip, op.cmd.code );
		vm->ip += op.cmd.offset;
//		ret = 1;
	} /* addr ok?*/

	return ret;
}

static
void
vvvm_run(
	struct vvvm_t * const vm )
{
	int	steps;
	int	r;

	steps = 0;
	do {
		r = vvvm_step( vm );
		steps++;
	} while ( r && ( steps < 100 ) );

	printf( "VVVM steps: %u\n", steps );
}

void
contest_vvvm(
	const struct contest_data_t * const bmp )
{
	struct vvvm_t	vm;

	vm.ip = 32;
	printf( "S=%zd\n", sizeof( union vvvm_opcode_t ) );
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
