/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_group.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:00:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/12 20:55:10 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static bool	handle_group_redirection(t_token **tokens, t_token **redir_head,
		t_token **redir_tail)
{
	append_token(redir_head, redir_tail, *tokens);
	(*tokens) = (*tokens)->next;
	if ((*tokens) && (*tokens)->type == T_WORD)
	{
		append_token(redir_head, redir_tail, *tokens);
		(*tokens) = (*tokens)->next;
		return (true);
	}
	return (false);
}

static bool	parse_group_redirections(t_token **tokens, t_node *group)
{
	t_token	*redir_head;
	t_token	*redir_tail;

	redir_head = NULL;
	redir_tail = NULL;
	while (*tokens && is_redirection((*tokens)->type))
	{
		if (!handle_group_redirection(tokens, &redir_head, &redir_tail))
		{
			free_tokens(redir_head);
			parser_error("syntax error after redirection");
			return (false);
		}
	}
	group->redir_list = redir_head;
	return (true);
}

t_node	*parse_group(t_token **tokens)
{
	t_node	*group;

	if (!tokens || !(*tokens) || (*tokens)->type != T_PAREN_OPEN)
		return (parser_error("syntax error: expected '('"), NULL);
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type == T_PAREN_CLOSE)
		return (parser_error("syntax error: unexpected ')'"), NULL);
	group = create_node(N_GROUP);
	if (!group)
		return (NULL);
	group->left = parse_and_or(tokens);
	if (!group->left)
		return (free(group), NULL);
	if (!(*tokens) || (*tokens)->type != T_PAREN_CLOSE)
	{
		free_node(group);
		return (parser_error("syntax error: expected ')'"), NULL);
	}
	(*tokens) = (*tokens)->next;
	if (!parse_group_redirections(tokens, group))
	{
		free_node(group);
		return (NULL);
	}
	return (group);
}
