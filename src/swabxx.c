
#include <woody.h>

uint32_t swab32( uint32_t dw )
{
	uint8_t *in;
	uint8_t *out;
	uint32_t wd;

	in = ( uint8_t * ) &dw;
	out = ( uint8_t * ) &wd;

	out[0] = in[3];
	out[1] = in[2];
	out[2] = in[1];
	out[3] = in[0];

	return wd;
}

uint64_t swab64( uint64_t qw )
{
	uint8_t *in;
	uint8_t *out;
	uint64_t wq;

	in = ( uint8_t * ) &qw;
	out = ( uint8_t * ) &wq;

	out[0] = in[7];
	out[1] = in[6];
	out[2] = in[5];
	out[3] = in[4];
	out[4] = in[3];
	out[5] = in[2];
	out[6] = in[1];
	out[7] = in[0];

	return wq;
}
