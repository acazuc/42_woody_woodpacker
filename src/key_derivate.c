#include "woody.h"

void key_derivate_8(t_env *env, char *key)
{
	env->key.q = 0x0123456789ABCDEF;
	uint64_t rot = 0x071F3B5989B3C7EF;
	for (size_t i = 0; i < ft_strlen(key); ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			env->key.b[j] += key[i] * (rot & 0xff);
			rot = (rot >> 12) | (rot << (64 - 12));
		}
		rot = (rot >> (8 * key[i])) | (rot << (64 - (8 * key[i])));
	}
}

void key_derivate_1(t_env *env, char *key)
{
	env->key.b[0] = 0x01;
	uint64_t rot = 0x071F3B5989B3C7EF;
	for (size_t i = 0; i < ft_strlen(key); ++i)
	{
		env->key.b[0] += key[i] * (rot & 0xff);
		rot = (rot >> (8 * key[i])) | (rot << (64 - (8 * key[i])));
	}
}
