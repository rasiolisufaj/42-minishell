/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:46:23 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/10 12:17:40 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	tokenizer(const char *input, int *i, t_token *new_token)
{
	if (input[*i] == '|')
		pipe_token(input, i, new_token);
	else if (input[*i] == '<')
		redir_less_token(input, i, new_token);
	else if (input[*i] == '>')
		redir_great_token(input, i, new_token);
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		new_token->type = T_AND;
		new_token->value = ft_strdup("&&");
		*i += 2;
	}
	else if (input[*i] == '&')
	{
		fprintf(stderr, "Error: '&' is not a valid token\n");
		exit(1);
	}
	else if (input[*i] == '(' || input[*i] == ')')
		parenthesis_token(input, i, new_token);
	else
		word_token(input, i, new_token);
}

void	head_or_tail(t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		*tail = new_token;
	}
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->value);
		free(tmp);
	}
}
