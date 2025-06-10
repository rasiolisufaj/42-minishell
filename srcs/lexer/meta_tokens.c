/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:44:01 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/10 12:17:19 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	pipe_token(const char *input, int *i, t_token *new_token)
{
	if (input[*i + 1] == '|')
	{
		new_token->type = T_OR;
		new_token->value = ft_strdup("||");
		*i += 2;
	}
	else
	{
		new_token->type = T_PIPE;
		new_token->value = ft_strdup("|");
		(*i)++;
	}
}

void	redir_less_token(const char *input, int *i, t_token *new_token)
{
	if (input[*i + 1] == '<')
	{
		new_token->type = T_HEREDOC;
		new_token->value = ft_strdup("<<");
		*i += 2;
	}
	else
	{
		new_token->type = T_REDIR_IN;
		new_token->value = ft_strdup("<");
		(*i)++;
	}
}

void	redir_great_token(const char *input, int *i, t_token *new_token)
{
	if (input[*i + 1] == '>')
	{
		new_token->type = T_REDIR_APPEND;
		new_token->value = ft_strdup(">>");
		*i += 2;
	}
	else
	{
		new_token->type = T_REDIR_OUT;
		new_token->value = ft_strdup(">");
		(*i)++;
	}
}

void	parenthesis_token(const char *input, int *i, t_token *new_token)
{
	if (input[*i] == '(')
	{
		new_token->type = T_PAREN_OPEN;
		new_token->value = ft_strdup("(");
		(*i)++;
	}
	else if (input[*i] == ')')
	{
		new_token->type = T_PAREN_CLOSE;
		new_token->value = ft_strdup(")");
		(*i)++;
	}
}
