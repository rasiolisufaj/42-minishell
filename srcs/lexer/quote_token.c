/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:50:44 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/10 14:52:05 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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

char	*concat_quote(const char *input, int *i, char *quoted)
{
	int		start;
	char	*after;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	if (*i == start)
		return (quoted);
	after = ft_substr(input, start, *i - start);
	return (join_and_free(quoted, after));
}
