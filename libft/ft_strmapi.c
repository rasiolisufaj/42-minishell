/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:47:51 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/18 17:02:47 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	int		i;

	if (!s || !f)
		return (0);
	str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (0);
	i = 0;
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*char	test_func(unsigned int i, char c)
{
	return c + i;
}

#include <stdio.h>
int	main()
{
	const char	*original = "hello there";
	char *result = ft_strmapi(original, test_func);
	printf("Original: %s\n", original);
	printf("Result: %s\n", result);
	free(result);
	return (0);
}*/