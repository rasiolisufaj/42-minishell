/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 00:50:00 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 00:53:28 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	process_non_expanded_token(t_token **head, t_token **tail,
		t_token *current)
{
	current->next = NULL;
	if (!*head)
	{
		*head = current;
		*tail = current;
	}
	else
	{
		(*tail)->next = current;
		*tail = current;
	}
}

void	expand_wildcards_in_tokens(t_token **token_list)
{
	t_token	*head;
	t_token	*tail;
	t_token	*current;
	t_token	*expanded;
	t_token	*next;

	head = NULL;
	tail = NULL;
	current = *token_list;
	while (current)
	{
		next = current->next;
		if (should_expand_wildcard(current))
		{
			expanded = handle_joker(current->value);
			if (expanded)
				process_expanded_token(&head, &tail, expanded, current);
			else
				process_non_expanded_token(&head, &tail, current);
		}
		else
			process_non_expanded_token(&head, &tail, current);
		current = next;
	}
	*token_list = head;
}

// Returns 1 when the pattern matches several files (ambiguous redirect).
int	process_redir_wildcard_token(t_token **head, t_token **tail,
		t_token *current)
{
	t_token	*expanded;

	expanded = handle_joker(current->value);
	if (expanded && expanded->next)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->value, 2);
		ft_putendl_fd(": ambiguous redirect", 2);
		free_tokens(expanded);
		process_non_expanded_token(head, tail, current);
		return (1);
	}
	if (expanded)
		process_expanded_token(head, tail, expanded, current);
	else
		process_non_expanded_token(head, tail, current);
	return (0);
}

int	expand_wildcards_in_redir(t_token **redir_list)
{
	t_token	*head;
	t_token	*tail;
	t_token	*current;
	t_token	*next;
	int		ambiguous;

	head = NULL;
	tail = NULL;
	ambiguous = 0;
	current = *redir_list;
	while (current)
	{
		next = current->next;
		if (should_expand_wildcard(current))
			ambiguous |= process_redir_wildcard_token(&head, &tail, current);
		else
			process_non_expanded_token(&head, &tail, current);
		current = next;
	}
	*redir_list = head;
	return (ambiguous);
}

// Returns 1 if a redirect is ambiguous; the command must then not run.
int	wildcards_cmd(t_node *node)
{
	expand_wildcards_in_tokens(&node->tokens);
	return (expand_wildcards_in_redir(&node->redir_list));
}
