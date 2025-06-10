/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:38:11 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/18 17:50:27 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return ;
	while (*s != 0)
	{
		f(i, s);
		i++;
		s++;
	}
}

/*#include <stdio.h>

void to_uppercase(unsigned int i, char *c)
{
	(void)i;
    if (*c >= 'a' && *c <= 'z')
        *c -= 32;
}

int main()
{
    char str[] = "hello, there! Libft Check!";
    ft_striteri(str, to_uppercase);
    printf("%s\n", str);
    return 0;
}*/