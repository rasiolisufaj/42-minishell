/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:50:44 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/20 18:51:04 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static void	append_segment(const char *input, int start, int end, char **buffer)
{
	char	*part;
	char	*tmp;

	part = ft_strndup(input + start, end - start);
	if (!part)
		return ;
	tmp = ft_strjoin(*buffer, part);
	free(*buffer);
	free(part);
	*buffer = tmp;
}

static void	handle_escape_sequence(const char *input, int *i, int *start,
		char **buffer)
{
	if (*i > *start)
		append_segment(input, *start, *i, buffer);
	else
		(*i)++;
	handle_escape(input, i, buffer);
	*start = *i;
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

void	handle_quote(const char *input, int *i, char **buffer)
{
	int		start;
	char	quote;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '"' && input[*i] == '\\' && input[*i + 1])
			handle_escape_sequence(input, i, &start, buffer);
		else
			(*i)++;
	}
	if (input[*i] == '\0')
		parser_error("syntax_error: unclosed quote");
	if (*i > start)
		append_segment(input, start, *i, buffer);
	if (input[*i] == quote)
		(*i)++;
}
