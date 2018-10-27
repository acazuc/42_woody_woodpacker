#include "woody.h"

int ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char*)s1)[i] != ((unsigned char*)s2)[i])
		{
			return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
		}
		i++;
	}
	return (0);
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
		i++;
	}
	return (dst);
}

int buffer_read(t_buffer *buffer, void *addr, size_t len)
{
	if (buffer->pos > buffer->len || buffer->pos + len > buffer->len)
		return (0);
	ft_memcpy(addr, buffer->data + buffer->pos, len);
	buffer->pos += len;
	return (1);
}

void ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char*)s)[i] = (unsigned char)0;
		i++;
	}
}

char ft_strcmp(char *s1, char *s2)
{
	size_t i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
}

size_t ft_strlen(char *s)
{
	size_t i = 0;
	while (s[i])
		++i;
	return i;
}
