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
vvvm_put(
	struct vvvm_t * const vm,
	const signed int offset,
	const union vvvm_opcode_t * const op )
{
	int	ret;

	if ( ( vm->ip + offset ) >= VVVM_SIZE ) {
		ret = 0;
	} else { /* addr ok */
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
	if ( !c ) {
	printf( "DIV zero\n" );

		return 0;
	}

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

	if ( g1 && g2 && g3 ) {
		g2 = vvvm_get( vm, op2.offset, &x );
		if ( g2 ) {
			f = func( &( z.word ), x.word, y.word );
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

	if ( g1 && g2 && g3 ) {
		if ( x.offset < y.offset ) {
			*jadd = jmp.offset;
		} /* compare? */
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

	if ( g1 ) {
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
		ret = vvvm_cmd_binop( vm, vvvm_binop_plus );
		break;
	case 2:
		ret = vvvm_cmd_binop( vm, vvvm_binop_minus );
		break;
	case 3:
		ret = vvvm_cmd_binop( vm, vvvm_binop_mul );
		break;
	case 4:
		ret = vvvm_cmd_binop( vm, vvvm_binop_div );
		break;
	case 5:
		ret = vvvm_cmd_binop( vm, vvvm_binop_and );
		break;
	case 6:
		ret = vvvm_cmd_binop( vm, vvvm_binop_or );
		break;
	case 7:
		ret = vvvm_cmd_binop( vm, vvvm_binop_shl );
		break;
	case 8:
		ret = vvvm_cmd_binop( vm, vvvm_binop_shr );
		break;
	case 9:
		ret = vvvm_cmd_op_not( vm );
		break;
	case 10:
		ret = vvvm_cmd_op_jmp( vm, &jadd );
		break;
	case 11:
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
		ret = vvvm_cmd( vm, op.cmd.code, op.cmd.jump );
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
	} while ( r && ( steps < 2000000000 ) );

	printf( "VVVM steps: %u\n", steps );
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
