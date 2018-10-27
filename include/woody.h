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
# include "colors.h"

# define ERROR(s) {dprintf(2, RED s);exit(EXIT_FAILURE);}

typedef struct s_buffer
{
	void *data;
	size_t len;
	size_t pos;
} t_buffer;

typedef struct s_segment_list
{
	Elf64_Phdr header;
	t_buffer buffer;
	struct s_segment_list *next;
} t_segment_list;

typedef struct s_section_list
{
	Elf64_Shdr header;
	t_buffer buffer;
	struct s_section_list *next;
} t_section_list;

typedef struct s_elf
{
	Elf64_Ehdr header;
	t_section_list *sections;
	t_segment_list *segments;
} t_elf;

typedef struct s_env
{
	void *bin;
	size_t bin_len;
	size_t bin_pos;
	uint64_t crypt_vstart;
	uint64_t crypt_len;
	uint64_t endpoint;
	uint64_t *start_addr;
	uint64_t *sect_off;
	Elf64_Shdr new_sec_hdr;
	void *new_sec_data;
	uint64_t new_sec_hdr_pos;
	union
	{
		uint8_t b[8];
		uint64_t q;
	} key;
	int askkey;
	int algo;
	t_elf elf;
} t_env;

int buffer_read(t_buffer *buffer, void *addr, size_t len);
void parse_file(t_env *env);
void crypt_file(t_env *env);
void file_output(t_env *env);
void prepare_decrypt(t_env *env);
void inject(t_env *env);

void key_derivate_1(t_env *env, char *key);
void key_derivate_8(t_env *env, char *key);

void print_file(t_env *env);
void print_ehdr(t_env *env, Elf64_Ehdr *hdr);
void print_phdr_header();
void print_phdr(t_env *env, Elf64_Phdr *hdr);
void print_shdr_header();
void print_shdr(t_env *env, Elf64_Shdr *hdr, int idx);

int ft_memcmp(const void *a1, const void *a2, size_t len);
void *ft_memcpy(void *dst, const void *src, size_t n);
void ft_bzero(void *s, size_t n);
char ft_strcmp(char *s1, char *s2);
size_t ft_strlen(char *s);

#endif
