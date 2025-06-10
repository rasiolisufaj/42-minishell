/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:42:38 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/13 15:43:19 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int c, size_t numbyte)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)dest;
	while (i < numbyte)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (dest);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char arr[10];

// 	ft_memset(arr, 'R', sizeof(arr));

// 	for (int i = 0; i < 10; i++)
// 		printf("%c ", arr[i]);
// 	printf("\n");

// 	return (0);
// }
