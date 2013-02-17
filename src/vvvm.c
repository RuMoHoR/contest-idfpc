#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "contest.h"

#define	VVVM_SIZE	13371111

union vvvm_opcode_t {
	struct {
		signed short jump;
		unsigned short code;
	} cmd;
	unsigned int	word;
	signed int	offset;
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
	const signed int offset,
	union vvvm_opcode_t * const op )
{
	int	ret;

	if ( ( vm->ip + offset ) >= VVVM_SIZE ) {
//		printf( "GET: Bad IP+Offs=%u+%d=%u\n",
//			vm->ip, offset, vm->ip + offset );
		op->word = 0;
		ret = 0;
	} else { /* addr ok */
//		printf( "GET: IP+Offs=%u+%d=[%u]=%08X(%d)\n",
//			vm->ip, offset, vm->ip + offset,
//			vm->data[ vm->ip + offset ].word,
//			vm->data[ vm->ip + offset ].offset );
		*op = vm->data[ vm->ip + offset ];
		ret = 1;
	} /* addr ok? */

	return ret;
}

static
int
vvvm_put(
	struct vvvm_t * const vm,
	const signed int offset,
	const union vvvm_opcode_t * const op )
{
	int	ret;

	if ( ( vm->ip + offset ) >= VVVM_SIZE ) {
//		printf( "PUT: Bad IP+Offs=%u+%d=%u\n",
//			vm->ip, offset, vm->ip + offset );
		ret = 0;
	} else { /* addr ok */
//		printf( "PUT: IP+Offs=%u+%d=[%u]=%08X(%d)\n",
//			vm->ip, offset, vm->ip + offset,
//			op->word, op->offset );
		vm->data[ vm->ip + offset ] = *op;
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
	int (*func)(
		unsigned int *a,
		const unsigned int b,
		const unsigned int c ) )
{
	union vvvm_opcode_t	op1;
	union vvvm_opcode_t	op2;
	union vvvm_opcode_t	x;
	union vvvm_opcode_t	y;
	union vvvm_opcode_t	z;
	int	g1, g2, g3, f;
	int	ret;

	ret = 0;
	g1 = vvvm_get( vm, 1, &op1 );
	g2 = vvvm_get( vm, 2, &op2 );
	g3 = vvvm_get( vm, 3, &y );

//	printf( "BIN: ofs %10d(%d) %10d(%d) %10d(%d)\n",
//		op1.offset, g1,
//		op2.offset, g2,
//		y.offset, g3 );

	if ( g1 && g2 && g3 ) {
		g2 = vvvm_get( vm, op2.offset, &x );
//	printf( "BIN: val %10d(%d)\n",
//		x.offset, g2 );
		if ( g2 && g3 ) {
			f = func( &( z.word ), x.word, y.word );
//	printf( "BIN: fun %10d(%d)\n",
//		z.offset, f );
			if ( f ) {
				ret = vvvm_put( vm, op1.word, &z );
			} /* func ok? */
		} /* operands ok? */
	} /* offsets ok? */

	return ret;
}

static
int
vvvm_cmd_op_not(
	struct vvvm_t * const vm )
{
	union vvvm_opcode_t	op1;
	union vvvm_opcode_t	x;
	union vvvm_opcode_t	y;
	int	g1, g2;
	int	ret;

	ret = 0;
	g1 = vvvm_get( vm, 1, &op1 );
	g2 = vvvm_get( vm, 2, &x );

//	printf( "BIN: ofs %10d(%d) %10d(%d) %10d(%d)\n",
//		op1.offset, g1,
//		op2.offset, g2,
//		y.offset, g3 );

	if ( g1 && g2 ) {
		y.word = ~x.word;
		ret = vvvm_put( vm, op1.word, &y );
	} /* offsets ok? */

	return ret;
}

static
int
vvvm_cmd_op_jmp(
	struct vvvm_t * const vm,
	signed int * const jadd )
{
	union vvvm_opcode_t	jmp;
	union vvvm_opcode_t	x;
	union vvvm_opcode_t	y;
	int	g1, g2, g3;
	int	ret;

	ret = 0;
	g1 = vvvm_get( vm, 1, &jmp );
	g2 = vvvm_get( vm, 2, &x );
	g3 = vvvm_get( vm, 3, &y );

//	printf( "BIN: ofs %10d(%d) %10d(%d) %10d(%d)\n",
//		op1.offset, g1,
//		op2.offset, g2,
//		y.offset, g3 );

	if ( g1 && g2 && g3 ) {
		if ( x.offset < y.offset ) {
			*jadd = jmp.offset;
		} /* operands ok? */
		ret = 1;
	} /* offsets ok? */

	return ret;
}

static
int
vvvm_cmd_op_print(
	struct vvvm_t * const vm )
{
	union vvvm_opcode_t	x;
	int	g1;
	int	ret;

	ret = 0;
	g1 = vvvm_get( vm, 1, &x );

//	printf( "BIN: ofs %10d(%d) %10d(%d) %10d(%d)\n",
//		op1.offset, g1,
//		op2.offset, g2,
//		y.offset, g3 );

	if ( g1 ) {
//		printf( "PRN[%c]\n", ( x.word & 0xFF ) );
		printf( "%c", ( x.word & 0xFF ) );
		ret = 1;
	} /* offsets ok? */

	return ret;
}

static
int
vvvm_cmd(
	struct vvvm_t * const vm,
	const unsigned short cmd,
	const signed short jmp )
{
	int	ret;
	signed int	jadd;

	ret = 1;
	jadd = jmp;

	switch ( cmd ) {
	case 1:
//		printf( "CMD: +\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_plus );
		break;
	case 2:
//		printf( "CMD: -\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_minus );
		break;
	case 3:
//		printf( "CMD: *\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_mul );
		break;
	case 4:
//		printf( "CMD: /\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_div );
		break;
	case 5:
//		printf( "CMD: &\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_and );
		break;
	case 6:
//		printf( "CMD: |\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_or );
		break;
	case 7:
//		printf( "CMD: <\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_shl );
		break;
	case 8:
//		printf( "CMD: >\n" );
		ret = vvvm_cmd_binop( vm, vvvm_binop_shr );
		break;
	case 9:
//		printf( "CMD: ~\n" );
		ret = vvvm_cmd_op_not( vm );
		break;
	case 10:
//		printf( "CMD: J\n" );
		ret = vvvm_cmd_op_jmp( vm, &jadd );
		break;
	case 11:
//		printf( "CMD: P\n" );
		ret = vvvm_cmd_op_print( vm );
		break;
	default:
		ret = 0;
		break;
	} /* sw cmd */

	vm->ip += jadd;

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
//		printf( "IP=%u C=%08X cmd=%04X offs=%d\n",
//			vm->ip, op.word, op.cmd.code, op.cmd.jump );
		ret = vvvm_cmd( vm, op.cmd.code, op.cmd.jump );
//		vm->ip += op.cmd.jump;
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
//	printf( "\nVVVM step: %u\n", steps );
		r = vvvm_step( vm );
		steps++;
//	printf( "\n" );
	} while ( r /*&& ( steps < 200000000 )*/ );

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
