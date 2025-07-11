/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:50:56 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/11 22:28:01 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static char	*create_escape_part(const char *input, int *i)
{
	char	*part;

	if (input[*i + 1] == '$')
	{
		part = malloc(3);
		if (!part)
			return (NULL);
		part[0] = input[*i];
		part[1] = input[*i + 1];
		part[2] = '\0';
		*i += 2;
	}
	else
	{
		part = malloc(2);
		if (!part)
			return (NULL);
		(*i)++;
		part[0] = input[*i];
		part[1] = '\0';
		(*i)++;
	}
	return (part);
}

void	handle_escape(const char *input, int *i, char **buffer)
{
	char	*part;
	char	*tmp;

	part = create_escape_part(input, i);
	if (!part)
		return (ft_putendl_fd("Error: Memory allocation failed", 2));
	tmp = ft_strjoin(*buffer, part);
	if (!tmp)
	{
		free(part);
		ft_putendl_fd("Error: Memory allocation failed", 2);
		return ;
	}
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

bool	word_token(const char *input, int *i, t_token *new_token)
{
	char	*buffer;

	buffer = malloc(1);
	if (!buffer)
		return (ft_putendl_fd("Error: Memory allocation failed", 2), false);
	buffer[0] = '\0';
	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]))
	{
		if (input[*i] == '\\')
			handle_escape(input, i, &buffer);
		else if (input[*i] == '\'' || input[*i] == '"')
		{
			if (!handle_quote(input, i, &buffer))
				return (free(buffer), false);
		}
		else
		{
			handle_unquoted(input, i, &buffer);
			if (input[*i] && is_space(input[*i]))
				break ;
		}
	}
	new_token->type = T_WORD;
	new_token->value = buffer;
	return (true);
}
