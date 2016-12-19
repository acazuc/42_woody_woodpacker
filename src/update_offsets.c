#include "woody.h"

void update_offsets(t_env *env)
{
	Elf64_Shdr sec_hdr;
	t_buffer buf;
	buf.data = env->bin;
	buf.pos = env->header.e_shoff;
	buf.len = env->bin_len;
	uint64_t add = env->new_sec_hdr.sh_size;
	if (add % 16)
		add += 16 - (add % 16);
	for (uint64_t i = 0; i < env->header.e_shnum; ++i)
	{
		if (!buffer_read(&buf, &sec_hdr, sizeof(sec_hdr)))
			ERROR("Invalid binary size can't read section header\n");
		if (sec_hdr.sh_offset >= env->new_sec_hdr.sh_offset)
		{
			uint64_t *offset = env->bin + buf.pos - sizeof(sec_hdr) + 4 + 4 + 8 + 8;
			*offset += add;
		}
	}
	uint64_t segid = 0;
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
		uint64_t soff = phdr.p_paddr + phdr.p_memsz;
		if (phdr.p_type == 1 && soff == env->endpoint)
		{
			uint64_t *fsize = env->bin + buf.pos - sizeof(phdr) + 4 + 4 + 8 + 8 + 8;
			uint64_t *msize = (void*)fsize + 8;
			add +=
			printf("Added %lu\n", add);
			*fsize += add;
			*msize += add;
			printf("fsize: %lu, mesize: %lu\n", *fsize, *msize);
			//break;
		}
		if (phdr.p_offset >= env->new_sec_hdr.sh_offset)
		{
			uint64_t *off = env->bin + buf.pos - sizeof(phdr) + 4 + 4;
			*off += add;//env->new_sec_hdr.sh_size;
		}
		printf("type: %d, flags: %d, offset: %ld, vaddr: %lu, paddr: %lu, filesz: %lu, memsz: %lu, align: %lu\n", phdr.p_type, phdr.p_flags, phdr.p_offset, phdr.p_vaddr, phdr.p_paddr, phdr.p_filesz, phdr.p_memsz, phdr.p_align);
	}
}
