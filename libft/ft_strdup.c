/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:42:28 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/16 12:46:15 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	int		i;
	char	*dup;
	size_t	str_len;

	i = 0;
	str_len = ft_strlen(src);
	dup = malloc(sizeof(char) * str_len + 1);
	if (!dup)
		return (NULL);
	while (src[i] != '\0')
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/*#include <stdio.h>

int	main(void)
{
	char	src[] = "HELLO WORLD!";
	printf("%s\n", ft_strdup(src));
	return (0);
}*/
