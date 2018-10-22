#include "woody.h"

static void parse_segments(t_env *env)
{
	t_buffer buf;
	buf.data = env->bin;
	buf.len = env->bin_len;
	t_segment_list *prev = NULL;
	for (int i = 0; i < env->elf.header.e_phnum; ++i)
	{
		t_segment_list *segment = malloc(sizeof(*segment));
		if (!segment)
			ERROR("Failed to malloc segment\n");
		buf.pos = env->elf.header.e_phoff + i * env->elf.header.e_phentsize;
		if (!buffer_read(&buf, &segment->header, sizeof(segment->header)))
			ERROR("Invalid segment header\n");
		segment->buffer.data = env->bin;
		segment->buffer.pos = segment->header.p_offset;
		segment->buffer.len = segment->header.p_filesz;
		segment->next = NULL;
		if (!prev)
			env->elf.segments = segment;
		else
			prev->next = segment;
		prev = segment;
	}
}

static void parse_sections(t_env *env)
{
	t_buffer buf;
	buf.data = env->bin;
	buf.len = env->bin_len;
	t_section_list *prev = NULL;
	for (int i = 0; i < env->elf.header.e_shnum; ++i)
	{
		t_section_list *section = malloc(sizeof(*section));
		if (!section)
			ERROR("Failed to malloc section\n");
		buf.pos = env->elf.header.e_shoff + i * env->elf.header.e_shentsize;
		if (!buffer_read(&buf, &section->header, sizeof(section->header)))
			ERROR("Invalid section header\n");
		section->buffer.data = env->bin;
		section->buffer.pos = section->header.sh_offset;
		section->buffer.len = section->header.sh_size;
		section->next = NULL;
		if (!prev)
			env->elf.sections = section;
		else
			prev->next = section;
		prev = section;
	}
}

void parse_file(t_env *env)
{
	t_buffer buf;

	buf.data = env->bin;
	buf.pos = 0;
	buf.len = env->bin_len;
	if (!buffer_read(&buf, &env->elf.header, sizeof(env->elf.header)))
		ERROR("Invalid binary header\n");
	if (ft_memcmp(env->elf.header.e_ident, ELFMAG, 4))
		ERROR("Invalid binary magic number\n");
	parse_segments(env);
	parse_sections(env);
}
