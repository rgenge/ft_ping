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

uint16_t checksum(uint16_t *data, size_t size) {
  uint32_t sum = 0;

  for (size_t i = 0; i < size; ++i) {
    sum += data[i];
  }

  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return ~sum;
}

void settime (void *time)
{
    struct timeval ptr;

    gettimeofday(&ptr, 0);

    ft_memcpy(time, &ptr.tv_sec, 4);
}