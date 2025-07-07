/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:28:32 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/07 17:00:18 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static t_node	*pipe_syntax_error(t_node *left)
{
	free_node(left);
	parser_error("syntax error near unexpected token '|'");
	return (NULL);
}

static	t_node	*create_pipe_node(t_node *left, t_node *right)
{
	t_node	*pipe_node;

	pipe_node = create_node(N_PIPE);
	if (!pipe_node)
	{
		free_node(left);
		free_node(right);
		return (NULL);
	}
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_node	*parse_pipe(t_token **tokens)
{
	t_node	*left;
	t_node	*right;

	if (!tokens || !(*tokens))
		return (NULL);
	if ((*tokens)->type == T_PIPE)
		return (parser_error("syntax error near unexpected token '|'"), NULL);
	left = parse_cmd(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == T_PIPE)
	{
		(*tokens) = (*tokens)->next;
		if (!(*tokens) || (*tokens)->type == T_PIPE)
			return (pipe_syntax_error(left));
		right = parse_cmd(tokens);
		if (!right)
			return (pipe_syntax_error(left));
		left = create_pipe_node(left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}
