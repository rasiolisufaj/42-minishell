/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:02:44 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/12 17:02:44 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (ft_isalpha(c) && c >= 'a')
		c -= 32;
	return (c);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char	a;

// 	a = 'c';
// 	printf("%c\n", ft_toupper(a));
// 	return (0);
// }
