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
		perror(RED "woody_woodpacker: open");
		exit(EXIT_FAILURE);
	}
	if (fstat(fd, &sstat) == -1)
	{
		perror(RED "woody_woodpacker: fstat");
		exit(EXIT_FAILURE);
	}
	if (!S_ISREG(sstat.st_mode))
		ERROR("You must give a regular file\n");
	*bin_len = sstat.st_size;
	if ((*bin = mmap(NULL, *bin_len + 5000, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror(RED "woody_woodpacker: mmap");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void read_random_bytes(void *ptr, size_t len)
{
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		ERROR("Failed to open urandom\n");
	if (read(fd, ptr, len) != (ssize_t)len)
		ERROR("Failed to read from urandom\n");
	close(fd);
}

int main(int ac, char **av)
{
	t_env env;
	ft_bzero(&env, sizeof(env));
	env.algo = 1;
	char *file = NULL;
	char *key = NULL;
	for (int i = 1; i < ac; ++i)
	{
		if (!ft_strcmp(av[i], "--hard"))
		{
			env.algo = 2;
		}
		else if (!ft_strcmp(av[i], "--key"))
		{
			if (i + 1 >= ac)
				ERROR("woody_woodpacker: key expected after --key\n");
			key = av[i];
		}
		else if (!ft_strcmp(av[i], "--askkey"))
		{
			env.askkey = 1;
		}
		else
		{
			if (file)
				ERROR(RED "woody_woodpacker " WHITE "[" PEACHY "--hard" WHITE "] [" YELLOW "--askkey" WHITE "] [" GREEN "--key key" WHITE "] " BLUE "file\n" DEFAULT);
			file = av[i];
		}
	}
	if (!file)
		ERROR(RED "woody_woodpacker " WHITE "[" PEACHY "--hard" WHITE "] [" YELLOW "--askkey" WHITE "] [" GREEN "--key key" WHITE "] " BLUE "file\n" DEFAULT);
	if (key)
	{
		if (env.algo == 2)
			key_derivate_8(&env, key);
		else
			key_derivate_1(&env, key);
	}
	else
	{
		if (env.algo == 2)
		{
			read_random_bytes(&env.key.q, 8);
			printf(GREEN "Key: " CYAN "0x%016lX\n" DEFAULT, env.key.q);
		}
		else
		{
			read_random_bytes(&env.key.b[0], 1);
			printf(GREEN "Key: " CYAN "0x%02X\n" DEFAULT, env.key.b[0]);
		}
	}
	read_file(file, &env.bin, &env.bin_len);
	parse_file(&env);
	//print_file(&env);
	crypt_file(&env);
	prepare_decrypt(&env);
	decryptcodegen(&env);
	inject(&env);
	file_output(&env);
	return (EXIT_SUCCESS);
}
