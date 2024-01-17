#include "ping.h"

// Current time
struct timeval getnow (void)
{
    struct timeval now;

    gettimeofday(&now, 0);
    return now;
}

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	unsigned char	*dest_aux;
	unsigned char	*src_aux;
	size_t			index;

	if (!dest && !src)
		return (NULL);
	dest_aux = (unsigned char *)dest;
	src_aux = (unsigned char *)src;
	index = 0;
	while (index < size)
	{
		*dest_aux = *src_aux;
		dest_aux++;
		src_aux++;
		index++;
	}
	return (dest);
}