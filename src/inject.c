#include "woody.h"

void inject(t_env *env)
{
	env->new_sec_hdr.sh_name = 0;
	env->new_sec_hdr.sh_type = SHT_PROGBITS;
	env->new_sec_hdr.sh_flags = SHF_EXECINSTR | SHF_ALLOC;
	env->new_sec_hdr.sh_link = 0;
	env->new_sec_hdr.sh_info = 0;
	env->new_sec_hdr.sh_addralign = 16;
	t_segment_list *lpt = NULL;
	for (t_segment_list *lst = env->elf.segments; lst; lst = lst->next)
	{
		if (lst->header.p_type == PT_LOAD)
		{
			lpt = lst;
			lpt->header.p_flags = PF_X | PF_W | PF_R;
		}
	}
	if (!lpt)
		ERROR("Can't find PT_LOAD");
	env->new_sec_hdr.sh_offset = lpt->header.p_offset + lpt->header.p_filesz;
	env->new_sec_hdr.sh_addr = lpt->header.p_vaddr + lpt->header.p_memsz;
	t_section_list *lsec = NULL;
	int i = 0;
	unsigned int lsecid = 0;
	for (t_section_list *lst = env->elf.sections; lst; lst = lst->next)
	{
		if (lst->header.sh_addr >= lpt->header.p_vaddr && lst->header.sh_addr + lst->header.sh_size <= lpt->header.p_vaddr + lpt->header.p_memsz)
		{
			lsecid = i;
			lsec = lst;
		}
		++i;
	}
	if (!lsec)
		ERROR("Can't find section");
	lpt->header.p_memsz += env->new_sec_hdr.sh_size;
	lpt->header.p_filesz += env->new_sec_hdr.sh_size;
	env->elf.header.e_shnum++;
	if (env->elf.header.e_shoff > env->new_sec_hdr.sh_offset)
		env->elf.header.e_shoff += env->new_sec_hdr.sh_size;
	if (lsecid < env->elf.header.e_shstrndx)
		env->elf.header.e_shstrndx++;
	t_section_list *new = malloc(sizeof(*new));
	if (!new)
		ERROR("failed to malloc new section");
	new->buffer.data = ((char*)env->new_sec_data) - env->new_sec_hdr.sh_offset;
	new->buffer.pos = env->new_sec_hdr.sh_offset;
	new->buffer.len = env->new_sec_hdr.sh_size;
	new->header = env->new_sec_hdr;
	new->next = lsec->next;
	lsec->next = new;
	env->elf.header.e_entry = env->new_sec_hdr.sh_addr;
	for (t_section_list *lst = env->elf.sections; lst; lst = lst->next)
	{
		if (lst->header.sh_link > lsecid)
			lst->header.sh_link++;
		if (lst->header.sh_offset >= new->header.sh_offset && lst != new && lst->header.sh_type != SHT_NOBITS)
			lst->header.sh_offset += new->header.sh_size;
		if (lst->header.sh_addr > new->header.sh_addr && lst != new)
			lst->header.sh_addr += new->header.sh_size;
	}
	for (t_segment_list *lst = env->elf.segments; lst; lst = lst->next)
	{
		if (lst->header.p_offset > new->header.sh_offset)
		{
			lst->header.p_offset += new->header.sh_size;
			lst->header.p_vaddr += new->header.sh_size;
		}
	}
}
