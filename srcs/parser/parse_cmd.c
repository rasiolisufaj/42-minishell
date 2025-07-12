/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:01:36 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/12 22:47:52 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static void	init_token_lists(t_token_lists *lists)
{
	lists->args_head = NULL;
	lists->args_tail = NULL;
	lists->redir_head = NULL;
	lists->redir_tail = NULL;
}

static bool	handle_redirection(t_token **tokens, t_token **redir_head,
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
	if (!(*tokens))
		parser_error("syntax error near unexpected token `newline'");
	else if ((*tokens)->type == T_AND)
		parser_error("syntax error near unexpected token `&&'");
	else if ((*tokens)->type == T_OR)
		parser_error("syntax error near unexpected token `||'");
	else if ((*tokens)->type == T_PIPE)
		parser_error("syntax error near unexpected token `|'");
	else
		parser_error("syntax error near unexpected token");
	return (false);
}

static bool	collect_cmd_tokens(t_token **tokens, t_token_lists *lists)
{
	while (*tokens && ((*tokens)->type == T_WORD
			|| is_redirection((*tokens)->type)))
	{
		if (is_redirection((*tokens)->type))
		{
			if (!handle_redirection(tokens, &lists->redir_head,
					&lists->redir_tail))
				return (false);
		}
		else
		{
			append_token(&lists->args_head, &lists->args_tail, *tokens);
			(*tokens) = (*tokens)->next;
		}
	}
	return (true);
}

t_node	*parse_cmd(t_token **tokens)
{
	t_token_lists	lists;
	t_node			*node;

	if (!tokens || !*tokens)
		return (NULL);
	if (*tokens && (*tokens)->type == T_PAREN_OPEN)
		return (parse_group(tokens));
	init_token_lists(&lists);
	if (!collect_cmd_tokens(tokens, &lists))
	{
		free_tokens(lists.args_head);
		free_tokens(lists.redir_head);
		return (NULL);
	}
	node = create_node(N_CMD);
	if (!node)
	{
		free_tokens(lists.args_head);
		free_tokens(lists.redir_head);
		return (NULL);
	}
	node->tokens = lists.args_head;
	node->redir_list = lists.redir_head;
	return (node);
}
