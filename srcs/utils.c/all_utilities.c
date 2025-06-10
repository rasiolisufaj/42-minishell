/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utilities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:25:54 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/10 11:49:25 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	is_space(char c)
{
	return (c == 32 || c == 9);
}

int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '&' || c == '|' || c == '('
		|| c == ')');
}

char	*ft_strndup(const char *src, size_t n)
{
	size_t	i;
	char	*dst;

	if (!src)
		return (NULL);
	dst = malloc(n + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
