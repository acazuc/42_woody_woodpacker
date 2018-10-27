#include "woody.h"

static char *get_name(t_env *env, t_section_list *section)
{
	int i = 0;
	for (t_section_list *lst = env->elf.sections; lst; lst = lst->next)
	{
		if (i == env->elf.header.e_shstrndx)
		{
			if (section->header.sh_name > lst->header.sh_size)
				return (NULL);
			return ((char*)lst->buffer.data + lst->buffer.pos + section->header.sh_name);
		}
		++i;
	}
	return (NULL);
}

void crypt_file(t_env *env)
{
	for (t_section_list *lst = env->elf.sections; lst; lst = lst->next)
	{
		char *name = get_name(env, lst);
		if (!name)
			continue;
		if (ft_strcmp(name, ".text"))
			continue;
		if (env->algo == 1)
		{
			int8_t xor = env->key.b[0];
			for (size_t i = 0; i < lst->buffer.len; ++i)
			{
				uint8_t tmp = ((char*)lst->buffer.data)[lst->buffer.pos + i];
				((char*)lst->buffer.data)[lst->buffer.pos + i] ^= xor;
				xor = tmp;
			}
		}
		else
		{
			uint64_t xor = env->key.q;
			for (size_t i = 0; i < lst->buffer.len; ++i)
			{
				uint8_t tmp = ((char*)lst->buffer.data)[lst->buffer.pos + i];
				((char*)lst->buffer.data)[lst->buffer.pos + i] ^= xor;
				xor = (xor & ~0xff) | tmp;
				xor = (xor >> 31) | (xor << (64 - 31));
			}
		}
		return;
	}
	ERROR("no .text section\n");
}
