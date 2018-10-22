#include "woody.h"

static t_segment_list *get_last_pt_load(t_env *env)
{
	t_segment_list *last_pt = NULL;
	for (t_segment_list *lst = env->elf.segments; lst; lst = lst->next)
	{
		if (lst->header.p_type == PT_LOAD)
		{
			last_pt = lst;
			last_pt->header.p_flags = PF_X | PF_W | PF_R;
		}
	}
	return last_pt;
}

static t_section_list *get_last_section(t_env *env, t_segment_list *last_pt_load, uint32_t *last_sec_id)
{
	uint32_t i = 0;
	t_section_list *last_sec = NULL;
	for (t_section_list *lst = env->elf.sections; lst; lst = lst->next)
	{
		if (lst->header.sh_addr >= last_pt_load->header.p_vaddr && lst->header.sh_addr + lst->header.sh_size <= last_pt_load->header.p_vaddr + last_pt_load->header.p_memsz)
		{
			*last_sec_id = i;
			last_sec = lst;
		}
		++i;
	}
	return last_sec;
}

static void build_new_section(t_env *env, t_segment_list *last_pt_load)
{
	env->new_sec_hdr.sh_name = 0;
	env->new_sec_hdr.sh_type = SHT_PROGBITS;
	env->new_sec_hdr.sh_flags = SHF_EXECINSTR | SHF_ALLOC;
	env->new_sec_hdr.sh_link = 0;
	env->new_sec_hdr.sh_info = 0;
	env->new_sec_hdr.sh_entsize = 0;
	env->new_sec_hdr.sh_addralign = 16;
	env->new_sec_hdr.sh_offset = last_pt_load->header.p_offset + last_pt_load->header.p_memsz;
	env->new_sec_hdr.sh_addr = last_pt_load->header.p_vaddr + last_pt_load->header.p_memsz;
}

static void update_offsets(t_env *env, t_section_list *new, uint32_t last_sec_id)
{
	int update = 0;
	t_section_list *prv = NULL;
	for (t_section_list *lst = env->elf.sections; lst; prv = lst, lst = lst->next)
	{
		if (lst->header.sh_link > last_sec_id)
			lst->header.sh_link++;
		if (update && prv)
			lst->header.sh_offset = prv->header.sh_offset + prv->header.sh_size;
		if (lst == new)
			update = 1;
	}
}

void inject(t_env *env)
{
	t_segment_list *last_pt_load = get_last_pt_load(env);
	if (!last_pt_load)
		ERROR("Can't find PT_LOAD\n");
	uint32_t last_sec_id;
	t_section_list *last_section = get_last_section(env, last_pt_load, &last_sec_id);
	if (!last_section)
		ERROR("Can't find last section\n");

	env->elf.header.e_shnum++;
	build_new_section(env, last_pt_load);

	last_pt_load->header.p_memsz += env->new_sec_hdr.sh_size;
	last_pt_load->header.p_filesz = last_pt_load->header.p_memsz;

	if (env->elf.header.e_shoff > env->new_sec_hdr.sh_offset)
		env->elf.header.e_shoff += env->new_sec_hdr.sh_size;
	if (last_sec_id < env->elf.header.e_shstrndx)
		env->elf.header.e_shstrndx++;
	t_section_list *new = malloc(sizeof(*new));
	if (!new)
		ERROR("failed to malloc new section\n");
	new->buffer.data = ((char*)env->new_sec_data) - env->new_sec_hdr.sh_offset;
	new->buffer.pos = env->new_sec_hdr.sh_offset;
	new->buffer.len = env->new_sec_hdr.sh_size;
	new->header = env->new_sec_hdr;
	new->next = last_section->next;
	last_section->next = new;
	env->elf.header.e_entry = env->new_sec_hdr.sh_addr;
	update_offsets(env, new, last_sec_id);
	t_section_list *last = NULL;
	for (t_section_list *lst = env->elf.sections; lst; lst = lst->next)
	{
		last = lst;
		if (lst->header.sh_type == SHT_SYMTAB)
			lst->header.sh_link = 0;
	}
	env->elf.header.e_shoff = last->header.sh_offset + last->header.sh_size;
}
