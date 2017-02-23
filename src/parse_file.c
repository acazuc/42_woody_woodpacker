#include "woody.h"

static char *get_symname(t_env *env, int index)
{
	if (index < 0 || (unsigned)index > env->strsec.sh_size)
		return (NULL);
	return ((char*)env->bin + env->strsec.sh_offset + index);
}

void parse_file(t_env *env)
{
	Elf64_Shdr sec_hdr;
	t_buffer buf;

	buf.data = env->bin;
	buf.pos = 0;
	buf.len = env->bin_len;
	if (!buffer_read(&buf, &env->header, sizeof(env->header)))
		ERROR("Invalid binary header\n");
	if (ft_memcmp(env->header.e_ident, ELFMAG, 4))
		ERROR("Invalid binary magic number\n");
	uint16_t *sect_nb = env->bin + 16 + 2 + 2 + 4 + 8 + 8 + 8 + 4 + 2 + 2 + 2 + 2;
	*sect_nb = *sect_nb + 1;
	uint16_t *strndx = env->bin + sizeof(Elf64_Ehdr) - 2;
	(*strndx)++;
	env->sect_off = env->bin + 16 + 2 + 2 + 4 + 8 + 8;
	env->start_addr = env->bin + 16 + 2 + 2 + 4;
	buf.pos = env->header.e_shoff;
	env->crypt_start = ULONG_MAX;
	env->crypt_end = 0;
	env->sections = NULL;
	env->endpoint = 0;
	uint64_t maxAddr = 0;
	uint64_t maxAddrLen = 0;
	uint64_t maxPoint = 0;
	for (uint64_t i = 0; i < env->header.e_shnum; ++i)
	{
		if (!buffer_read(&buf, &sec_hdr, sizeof(sec_hdr)))
			ERROR("Invalid binary size can't read section header\n");
		{
			t_shdr_list *new = malloc(sizeof(*new));
			if (!new)
				ERROR("Malloc failed\n");
			new->next = env->sections;
			new->data = sec_hdr;
			env->sections = new;
		}
		if (sec_hdr.sh_addr > maxAddr)
		{
			maxAddr = sec_hdr.sh_addr;
			maxAddrLen = sec_hdr.sh_size;
			maxPoint = sec_hdr.sh_offset + sec_hdr.sh_size;
			env->endpoint = sec_hdr.sh_addr + sec_hdr.sh_size;
			env->new_sec_hdr_pos = buf.pos;
		}
		if (sec_hdr.sh_type == SHT_SYMTAB)
		{
			int *link = buf.data + buf.pos - sizeof(sec_hdr) + 4 + 4 + 8 + 8 + 8 + 8;
			*link = i + 1;
		}
		if (i == env->header.e_shstrndx)
			env->strsec = sec_hdr;
	}
	ft_bzero(&env->new_sec_hdr, sizeof(env->new_sec_hdr));
	env->new_sec_hdr.sh_offset = maxPoint;
	env->new_sec_hdr.sh_size = 0;
	env->new_sec_hdr.sh_type = SHT_PROGBITS;
	env->new_sec_hdr.sh_addralign = 16;
	env->new_sec_hdr.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
	env->new_sec_hdr.sh_addr = maxAddr + maxAddrLen;
	env->crypt_len = env->crypt_end - env->crypt_start;
	t_shdr_list *sect = env->sections;
	while (sect)
	{
		char *secname = get_symname(env, sect->data.sh_name);
		if (!secname)
			goto next;
		if (ft_memcmp(secname, ".text", 6))
			goto next;
		env->crypt_start = sect->data.sh_offset;
		env->crypt_vstart = sect->data.sh_addr;
		env->crypt_len = sect->data.sh_size;
		env->crypt_end = env->crypt_start + env->crypt_len;
		return;
		next:
		sect = sect->next;
	}
}
