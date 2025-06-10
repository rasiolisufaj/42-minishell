/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:52:52 by raisufaj          #+#    #+#             */
/*   Updated: 2025/01/30 14:52:52 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_type(char c, va_list args)
{
	if (c == 'c')
		return (print_char(va_arg(args, int)));
	else if (c == 's')
		return (print_string(va_arg(args, char *)));
	else if (c == 'd' || c == 'i')
		return (print_int(va_arg(args, int)));
	else if (c == 'x' || c == 'X')
		return (print_hex(va_arg(args, unsigned int), c));
	else if (c == 'p')
		return (print_pointer(va_arg(args, void *)));
	else if (c == 'u')
		return (print_unsigned(va_arg(args, unsigned int)));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		total_len;

	total_len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (ft_strchr("cspdiuxX", *format))
				total_len += check_type(*format, args);
			else if (*format == '%')
				total_len += print_char('%');
		}
		else
			total_len += print_char(*format);
		format++;
	}
	va_end(args);
	return (total_len);
}
