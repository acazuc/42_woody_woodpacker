
#include <pancake.h>

void quit( int code )
{
	remove( DECRYPTOUTPATH );
	exit( code );
}

void assemble( void )
{
	pid_t child;
	int bullshit;

	if ( child = fork() )
	{
		if ( child == -1 )
		{
			perror( PERRORPREFIX "Can't fork" );
			exit( EXIT_FAILURE );
		}
		wait( &bullshit );
	}
	else
	{
		char *argv[7] =
		{
			"nasm",
			DECRYPTINPATH,
			"-f",
			"elf64",
			"-o",
			DECRYPTOUTPATH,
			NULL
		};

		close( 0 );
		close( 1 );
		close( 2 );
		execvp( "nasm", argv );
		perror( PERRORPREFIX "Can't run assembler" );
		exit( EXIT_FAILURE );
	}
}

void *getbin( void )
{
	int fd;
	void *addr;
	struct stat bdts;

	if ( ( fd = open( DECRYPTOUTPATH, O_RDONLY ) ) == -1 )
	{
		perror( PERRORPREFIX "Can't open hex file" );
		quit( EXIT_FAILURE );
	}
	if ( fstat( fd, &bdts ) )
	{
		perror( PERRORPREFIX "Can't stat hex file" );
		quit( EXIT_FAILURE );
	}
	if ( ( addr = mmap( NULL, bdts.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0 ) ) == MAP_FAILED )
	{
		perror( PERRORPREFIX "Can't map hex file" );
		quit( EXIT_FAILURE );
	}
	return addr;
}

void decryptcodegen( t_env *env )
{
	void *addr;
	Elf64_Ehdr *header;
	Elf64_Shdr *sect;
	char *strtable;
	
	assemble();
	addr = getbin();

	if ( ft_memcmp( addr, ELFMAG, 4 ) )
	{
		fprintf( stderr, PERRORPREFIX "Bad hexfile" );
		quit( EXIT_FAILURE );
	}

	header = addr;
	sect = addr + (*header).e_shoff;
	strtable = addr + sect[(*header).e_shstrndx].sh_offset;
	for ( int i = 0 ; i < (*header).e_shnum ; i++ )
	{
		if ( !ft_memcmp( &strtable[(*sect).sh_name], ".text", 6 ) )
		{
			uint8_t *data = addr + (*sect).sh_offset;
			void *tail = data + (*sect).sh_size - 34;

			printf( "%p %p\n", *(*env).start_addr, (*env).new_sec_hdr.sh_addr );
			printf( "%llx %llx\n", (*env).crypt_start, (*env).crypt_end );
			printf( "%llx %x\n", (*env).crypt_vstart, ( (*env).crypt_end - (*env).crypt_start ) + (*env).crypt_vstart );

			*( uint32_t * ) tail = *(*env).start_addr - ( (*env).new_sec_hdr.sh_addr + (*sect).sh_size - 30 );
	UINT_MAX - ( uint64_t ) ( ( (*env).new_sec_hdr.sh_addr - ( uint64_t ) (*env).start_addr ) + ( tail - ( uint64_t ) data ) );
			*( uint64_t * ) ( tail + 4 ) = (*env).crypt_vstart;
			*( uint64_t * ) ( tail + 12 ) = ( (*env).crypt_end - (*env).crypt_start ) + (*env).crypt_vstart;

			(*env).new_sec_data = data;
			(*env).new_sec_hdr.sh_size = (*sect).sh_size;

			for ( int j = 0 ; j < (*sect).sh_size ; j++ )
			{
				printf( " %02hhx", *data );
				if ( ( j % 16 ) == 15 )
					puts( "\n" );
				data++;
			}
			putchar( '\n' );
			remove( DECRYPTOUTPATH );
			return;
		}
		sect++;
	}
	fprintf( stderr, PERRORPREFIX "Missing .text section in hexfile" );
	quit( EXIT_FAILURE );
}

/*int main( void )
{
	t_env env;

	ft_bzero( &env, sizeof ( t_env ) );

	env.start_addr = 0;
	env.new_sec_hdr.sh_addr = 0x66;
	env.crypt_start = 0x88888888;
	env.crypt_end = 0xabcdef00;

	decryptcodegen( &env );
	return EXIT_SUCCESS;
}*/
