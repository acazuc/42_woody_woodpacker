#include "woody.h"

void update_offsets(t_env *env)
{
	Elf64_Shdr sec_hdr;
	t_buffer buf;
	buf.data = env->bin;
	buf.pos = env->header.e_shoff;
	buf.len = env->bin_len;
	for (uint64_t i = 0; i < env->header.e_shnum; ++i)
	{
		if (!buffer_read(&buf, &sec_hdr, sizeof(sec_hdr)))
			ERROR("Invalid binary size can't read section header\n");
		if (buf.pos >= env->new_sec_hdr_pos)
		{
			uint64_t *offset = env->bin + buf.pos + 4 + 4 + 8 + 8;
			*offset += env->new_sec_hdr.sh_size;
		}
	}
	buf.pos = env->header.e_phoff;
	for (uint64_t i = 0; i < env->header.e_phnum; ++i)
	{
		Elf64_Phdr phdr;
		if (!buffer_read(&buf, &phdr, sizeof(phdr)))
			ERROR("Failed to read program header\n");
		if (phdr.p_type == 1)
		{
			int *flags = env->bin + buf.pos - sizeof(phdr) + 4;
			*flags = PF_X | PF_W | PF_R;
		}
		if (phdr.p_type == 1 && phdr.p_offset + phdr.p_filesz == env->endpoint - 8)
		{
			uint64_t *fsize = env->bin + buf.pos - sizeof(phdr) + 4 + 4 + 8 + 8 + 8;
			uint64_t *msize = (void*)fsize + 8;
			*fsize += 48;
			*msize += 48;
		}
		if (phdr.p_offset > 10000)
		{
			uint64_t *off = env->bin + buf.pos - sizeof(phdr) + 4 + 4;
			*off += 0;
		}
		//printf("type: %d, flags: %d, offset: %ld, vaddr: %lu, paddr: %lu, filesz: %lu, memsz: %lu, align: %lu\n", phdr.p_type, phdr.p_flags, phdr.p_offset, phdr.p_vaddr, phdr.p_paddr, phdr.p_filesz, phdr.p_memsz, phdr.p_align);
	}
}
