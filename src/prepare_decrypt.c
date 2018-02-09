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

void prepare_decrypt(t_env *env)
{
	env->start_addr = &env->elf.header.e_entry;
	for (t_section_list *lst = env->elf.sections; lst; lst = lst->next)
	{
		char *name = get_name(env, lst);
		if (!name)
			continue;
		if (ft_strcmp(name, ".text"))
			continue;
		env->crypt_vstart = lst->header.sh_addr;
		env->crypt_len = lst->header.sh_size;
		goto next;
	}
	ERROR("no .text section");
next:
	;
	t_segment_list *lpt = NULL;
	for (t_segment_list *lst = env->elf.segments; lst; lst = lst->next)
	{
		if (lst->header.p_type == PT_LOAD)
			lpt = lst;
	}
	if (!lpt)
		ERROR("Can't find PT_LOAD");
	env->new_sec_hdr.sh_addr = lpt->header.p_vaddr + lpt->header.p_memsz;
}
