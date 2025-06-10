/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:35:41 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/15 17:47:07 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*little == 0)
		return ((char *)big);
	while (i < len && big[i])
	{
		j = 0;
		while (i + j < len && big[i + j] && little[j] == big[i + j])
			j++;
		if (little[j] == '\0')
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}

/*#include <stdio.h>

int	main()
{
	char	str1[] = "jesuisuitessuitestesbklm";
	char	str2[] = "test";

	printf("%s\n", ft_strnstr(str1, str2, 18));
	return (0);
}*/
