/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:46:00 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/06 22:43:09 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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
		new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			free_tokens(head);
			return (NULL);
		}
		new_token->next = NULL;
		new_token->value = NULL;
		new_token->type = T_UNKNOWN;
		if (!tokenizer(input, &i, new_token))
		{
			free(new_token);
			free_tokens(head);
			return (NULL);
		}
		head_or_tail(&head, &tail, new_token);
	}
	return (head);
}
