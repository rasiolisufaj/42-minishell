/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:01:02 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/18 16:18:53 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_length(long nb)
{
	size_t	length;

	length = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		nb *= -1;
		length++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		length++;
	}
	return (length);
}

static char	*fill_str(char *str, long nbr, size_t len)
{
	if (nbr == 0)
	{
		str[0] = '0';
		return (str);
	}
	while (nbr > 0)
	{
		str[--len] = (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nbr;
	size_t	len;

	nbr = n;
	len = num_length(nbr);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nbr < 0)
	{
		str[0] = '-';
		nbr *= -1;
	}
	return (fill_str(str, nbr, len));
}

/*#include <stdio.h>

int main()
{
    int num1 = 1234;
    int num2 = -5678;
    int num3 = 0;
    int num4 = INT_MIN;

    char *str1 = ft_itoa(num1);
    char *str2 = ft_itoa(num2);
    char *str3 = ft_itoa(num3);
    char *str4 = ft_itoa(num4);

    printf("%s\n", str1);
    printf("%s\n", str2);
    printf("%s\n", str3);
    printf("%s\n", str4);

    free(str1);
    free(str2);
    free(str3);
    free(str4);

    return 0;
}*/
