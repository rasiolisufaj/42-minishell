/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:46:00 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/06 23:58:41 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static t_token	*create_and_init_token(const char *input, int *i,
		t_token **head)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free_tokens(*head);
		return (NULL);
	}
	new_token->next = NULL;
	new_token->value = NULL;
	new_token->type = T_UNKNOWN;
	if (tokenizer(input, i, new_token))
	{
		free(new_token);
		free_tokens(*head);
		return (NULL);
	}
	return (new_token);
}

t_token	*lexer(const char *input)
{
	int		i;
	t_token	*new_token;
	t_token	*head;
	t_token	*tail;

	i = 0;
	head = NULL;
	tail = NULL;
	while (input[i] != '\0')
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		new_token = create_and_init_token(input, &i, &head);
		if (!new_token)
			return (NULL);
		head_or_tail(&head, &tail, new_token);
	}
	return (head);
}
