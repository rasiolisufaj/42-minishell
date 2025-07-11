/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:50:56 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/09 15:26:53 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	handle_escape(const char *input, int *i, char **buffer)
{
	char	*part;
	char	*tmp;

	if (input[*i + 1] == '$')
	{
		part = malloc(3);
		if (!part)
			exit(1);
		part[0] = input[*i];
		part[1] = input[*i + 1];
		part[2] = '\0';
		*i += 2;
	}
	else
	{
		part = malloc(2);
		if (!part)
			exit(1);
		(*i)++;
		part[0] = input[*i];
		part[1] = '\0';
		(*i)++;
	}
	tmp = ft_strjoin(*buffer, part);
	free(*buffer);
	free(part);
	*buffer = tmp;
}

void	handle_unquoted(const char *input, int *i, char **buffer)
{
	int		start;
	char	*part;
	char	*tmp;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\'' && input[*i] != '"' && input[*i] != '\\')
		(*i)++;
	if (*i > start)
	{
		part = ft_strndup(input + start, (*i) - start);
		tmp = ft_strjoin(*buffer, part);
		free(*buffer);
		free(part);
		*buffer = tmp;
	}
}

void	word_token(const char *input, int *i, t_token *new_token)
{
	char	*buffer;

	buffer = malloc(1);
	if (!buffer)
		exit(1);
	buffer[0] = '\0';
	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]))
	{
		if (input[*i] == '\\')
			handle_escape(input, i, &buffer);
		else if (input[*i] == '\'' || input[*i] == '"')
			handle_quote(input, i, &buffer);
		else
		{
			handle_unquoted(input, i, &buffer);
			if (input[*i] && is_space(input[*i]))
				break ;
		}
	}
	new_token->type = T_WORD;
	new_token->value = buffer;
}
