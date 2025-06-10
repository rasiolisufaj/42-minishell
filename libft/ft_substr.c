/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:05:06 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/16 15:39:56 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start > slen)
	{
		len = 0;
		start = 0;
	}
	else if (slen < (start + len))
		len = slen - start;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*#include <stdio.h>
#include <string.h>

int main (void)
{
    char hello[] = "0123456789";
	char greetings[] = "Hello, world!";
    char *sub = ft_substr(hello, 9, 10);
	char *greetings_sub = ft_substr(greetings, 7, 1);

    printf("%s\n", sub);
	printf("%zu\n", ft_strlen(sub));
	printf("%s\n", greetings_sub);
	printf("%zu\n", ft_strlen(sub));
    free(sub);
    free(greetings_sub);
    return (0);

}*/