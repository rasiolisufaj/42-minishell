/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:23:02 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/15 14:13:52 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (s < d)
	{
		while (n--)
			d[n] = s[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dest);
}

/*#include <stdio.h>

int	main(void)
{
	char buffer[20] = "HelloWorld";
	char buffer1[20] = "HelloWorld";
	char buffer2[20] = "HelloWorld";
	char buffer3[20] = "HelloWorld";
	char src[] = "12345";
	ft_memmove(buffer + 5, buffer, 5);
	printf("Buffer + 5 => %s\n", buffer);
	ft_memmove(buffer1 + 3, buffer1, 5);
	printf("Buffer + 3 => %s\n", buffer1);
	ft_memmove(buffer2 + 2, buffer2, 5);
	printf("Buffer + 2 => %s\n", buffer2);
	ft_memmove(buffer3, src, 5);
	printf("Buffer3 / SRC => %s\n", buffer3);
	return (0);
}*/