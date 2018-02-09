#include "woody.h"
#include <pancake.h>

ssize_t wreight( int fd, const void *buf, size_t len )
{
	int wc;

	while ( len )
	{
		if ( ( wc = write( fd, buf, len ) ) < 0 )
			return -1;
		len -= wc;
		buf += wc;
	}
	return 1;
}

void read_file(char *file, void **bin, size_t *bin_len)
{
	struct stat sstat;
	int fd;

	if ((fd = open(file, O_RDONLY)) == -1)
	{
		perror("woody_woodpacker: open");
		exit(EXIT_FAILURE);
	}
	if (fstat(fd, &sstat) == -1)
	{
		perror("woody_woodpacker: fstat");
		exit(EXIT_FAILURE);
	}
	if (!S_ISREG(sstat.st_mode))
		ERROR("You must give a regular file\n");
	*bin_len = sstat.st_size;
	if ((*bin = mmap(NULL, *bin_len + 5000, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("woody_woodpacker: mmap");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

int main(int ac, char **av)
{
	t_env env;
	ft_bzero(&env, sizeof(env));
	if (ac != 2)
		ERROR("You must specify a file to pack\n");
	read_file(av[1], &env.bin, &env.bin_len);
	parse_file(&env);
	print_file(&env);
	//crypt_file(&env);
	prepare_decrypt(&env);
	decryptcodegen(&env);
	inject(&env);
	file_output(&env);
	/**env.start_addr = env.new_sec_hdr.sh_addr;
	*env.sect_off += env.new_sec_hdr.sh_size;
	update_offsets(&env);
	int fd = open("woody", O_WRONLY | O_TRUNC | O_CREAT, 0755);
	if (fd == -1)
	{
		perror("woody_woodpacker: Can't open woody file");
		exit(EXIT_FAILURE);
	}
	if (wreight(fd, env.bin, env.new_sec_hdr.sh_offset) == -1)
	{
		perror("woody_woodpacker: Failed to write to woody 1");
		exit(EXIT_FAILURE);
	}
	if (wreight(fd, env.new_sec_data, env.new_sec_hdr.sh_size) == -1)
	{
		perror("woody_woodpacker: Failed to write new sect");
		exit(EXIT_FAILURE);
	}
	if (wreight(fd, env.bin + env.new_sec_hdr.sh_offset, env.new_sec_hdr_pos - env.new_sec_hdr.sh_offset) == -1)
	{
		perror("woody_woodpacker: Failed to write to woody 2");
		exit(EXIT_FAILURE);
	}
	if (wreight(fd, &env.new_sec_hdr, sizeof(env.new_sec_hdr)) == -1)
	{
		perror("woody_woodpacker: Failed to write new sect hdr");
		exit(EXIT_FAILURE);
	}
	if (wreight(fd, env.bin + env.new_sec_hdr_pos, env.bin_len - env.new_sec_hdr_pos) == -1)
	{
		perror("woody_woodpacker: Failed to write to woody 3");
		exit(EXIT_FAILURE);
	}
	close(fd);
	printf("Successfully written");*/
	return (EXIT_SUCCESS);
}
