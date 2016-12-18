#ifndef WOODY_H
# define WOODY_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <elf.h>

# define ERROR(s) {dprintf(2, s);exit(EXIT_FAILURE);}

typedef struct s_buffer
{
	void *data;
	size_t len;
	size_t pos;
} t_buffer;

typedef struct s_shdr_list
{
	Elf64_Shdr data;
	struct s_shdr_list *next;
} t_shdr_list;

typedef struct s_env
{
	void *bin;
	size_t bin_len;
	size_t bin_pos;
	uint64_t crypt_start;
	uint64_t crypt_end;
	uint64_t crypt_len;
	uint64_t endpoint;
	uint64_t *sect_off;
	void *new_sec_data;
	Elf64_Shdr new_sec_hdr;
	uint64_t new_sec_hdr_pos;
	Elf64_Ehdr header;
	Elf64_Shdr strsec;
	t_shdr_list *sections;
} t_env;

int buffer_read(t_buffer *buffer, void *addr, size_t len);
void parse_file(t_env *env);
void crypt_file(t_env *env);
void update_offsets(t_env *env);

int ft_memcmp(const void *a1, const void *a2, size_t len);
void *ft_memcpy(void *dst, const void *src, size_t n);
void ft_bzero(void *s, size_t n);

#endif
