#include "woody.h"

void file_output(t_env *env)
{
	ssize_t osef;
	int fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755);
	osef = write(fd, &env->elf.header, sizeof(env->elf.header));
	if (lseek(fd, env->elf.header.e_phoff, SEEK_SET) != (ssize_t)env->elf.header.e_phoff)
		ERROR("lseek failed");
	for (t_segment_list *segment = env->elf.segments; segment; segment = segment->next)
		osef = write(fd, &segment->header, sizeof(segment->header));
	int i = 0;
	for (t_section_list *section = env->elf.sections; section; section = section->next)
	{
		if (section->header.sh_type != SHT_NOBITS) // bss section
		{
			if (lseek(fd, section->header.sh_offset, SEEK_SET) != (ssize_t)section->header.sh_offset)
				ERROR("lseek failed");
			osef = write(fd, section->buffer.data + section->buffer.pos, section->buffer.len);
		}
		if (lseek(fd, env->elf.header.e_shoff + i * sizeof(section->header), SEEK_SET) != (ssize_t)(env->elf.header.e_shoff + i * sizeof(section->header)))
			ERROR("lseek failed");
		osef = write(fd, &section->header, sizeof(section->header));
		++i;
	}
	(void)osef;
}
