/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:02:40 by raisufaj          #+#    #+#             */
/*   Updated: 2025/05/14 14:11:27 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_hex(unsigned long n, char format)
{
	int		len;
	char	*base;

	len = 0;
	if (format == 'x')
		base = "0123456789abcdef";
	else if (format == 'X')
		base = "0123456789ABCDEF";
	else
		return (0);
	if (n >= 16)
		len += print_hex(n / 16, format);
	len += print_char(base[n % 16]);
	return (len);
}

int	print_int(int num)
{
	long			nb;
	unsigned int	count;

	nb = num;
	count = 1;
	if (nb < 0)
	{
		ft_putchar_fd('-', 1);
		nb = -nb;
		count++;
	}
	if (nb > 9)
		count += print_int(nb / 10);
	ft_putchar_fd(nb % 10 + '0', 1);
	if (num == -2147483648)
		return (11);
	return (count);
}

int	print_pointer(void *ptr)
{
	int				len;
	unsigned long	addr;

	if (ptr == NULL)
		return (print_string("(nil)"));
	len = 0;
	addr = (unsigned long)ptr;
	len += print_string("0x");
	len += print_hex(addr, 'x');
	return (len);
}

int	print_string(char *s)
{
	unsigned int	i;

	i = 0;
	if (!s)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	while (s[i])
		i++;
	ft_putstr_fd(s, 1);
	return (i);
}

int	print_unsigned(unsigned int n)
{
	int	len;

	len = 0;
	if (n >= 10)
		len += print_unsigned(n / 10);
	len += print_char((n % 10) + '0');
	return (len);
}
