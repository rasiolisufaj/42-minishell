/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utilities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:25:54 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/10 15:26:30 by raisufaj         ###   ########.fr       */
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

char	*join_and_free(char *s1, char *s2)
{
	char	*joined;
	size_t	total_len;

	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = malloc(sizeof(char) * total_len);
	if (!joined)
	{
		free(s1);
		free(s2);
		exit(1);
	}
	joined[0] = '\0';
	ft_strlcat(joined, s1, total_len);
	ft_strlcat(joined, s2, total_len);
	free(s1);
	free(s2);
	return (joined);
}
