/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:55:25 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/15 12:21:59 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	i = 0;
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char src[] = "Hello, World!";
// 	char ft_memcpy_dest[20];
// 	char memcpy_dest[20];

// 	ft_memcpy(ft_memcpy_dest, src, ft_strlen(src) + 1);
// 	memcpy(memcpy_dest, src, ft_strlen(src) + 1);

// 	printf("ft_memcpy => %s\n", ft_memcpy_dest);
// 	printf("memcpy => %s\n", memcpy_dest);
// 	return (0);
// }