#include "woody.h"

void crypt_file(t_env *env)
{
	int8_t xor = 0x94;
	for (uint64_t i = env->crypt_start; i < env->crypt_end; ++i)
	{
		((char*)env->bin)[i] = ((char*)env->bin)[i] ^ xor;
		xor = ((char*)env->bin)[i];
	}
}
