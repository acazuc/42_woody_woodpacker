
#ifndef PANCAKE_H
# define PANCAKE_H

# include <woody.h>
# include <sys/wait.h>

# define DECRYPTINPATH "decrypt.s"
# define DECRYPTOUTPATH "/tmp/woodywoodpackerhexa.o"
//# define DECRYPTOUTPATH "mooga.o"

# define PERRORPREFIX "woody_woodpacker: "

# define YAY puts( "Breakpoint passed yay :3" )

void quit( int code );
void assemble( void );
void *getbin( void );
void decryptcodegen( t_env *env );

#endif