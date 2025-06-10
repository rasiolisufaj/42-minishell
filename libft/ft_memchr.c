/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:43 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/15 17:29:10 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return ((void *)(str + i));
		i++;
	}
	return (NULL);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char str[] = "Try to find 1st position of letter e";
// 	char toFind = 'e';
// 	char *result = ft_memchr(str, toFind, 36);

// 	if (result != NULL)
// 		printf("Found '%c' at position: %ld\n", toFind, result - str);
// 	else
// 		printf("'%c' not found\n", toFind);
// 	return (0);
// }