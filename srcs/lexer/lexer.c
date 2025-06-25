/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:46:00 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/24 07:46:15 by itaharbo         ###   ########.fr       */
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
			exit(1);
		new_token->next = NULL;
		tokenizer(input, &i, new_token);
		head_or_tail(&head, &tail, new_token);
	}
	return (head);
}
