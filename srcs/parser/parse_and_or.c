/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_or.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:37:08 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/12 22:33:05 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static t_node	*syntax_error(t_token_type type)
{
	if (type == T_AND)
		parser_error("syntax error near unexpected token '&&'");
	else
		parser_error("syntax error near unexpected token '||'");
	return (NULL);
}

static t_node	*handle_parse_error(t_node *left, t_token_type type)
{
	free_node(left);
	return (syntax_error(type));
}

static t_node	*create_logic_node(t_token_type type, t_node *left,
		t_node *right)
{
	t_node	*node;

	if (type == T_AND)
		node = create_node(N_AND);
	else
		node = create_node(N_OR);
	if (!node)
	{
		free_node(left);
		free_node(right);
		return (NULL);
	}
	node->left = left;
	node->right = right;
	return (node);
}

t_node	*parse_and_or(t_token **tokens)
{
	t_node			*left;
	t_node			*right;
	t_token_type	type;

	if (!tokens || !(*tokens))
		return (NULL);
	if ((*tokens)->type == T_AND || (*tokens)->type == T_OR)
		return (syntax_error((*tokens)->type));
	left = parse_pipe(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->type == T_AND || (*tokens)->type == T_OR))
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		if (!*tokens || (*tokens)->type == T_AND || (*tokens)->type == T_OR)
			return (handle_parse_error(left, type));
		right = parse_pipe(tokens);
		if (!right)
			return (free_node(left), NULL);
		left = create_logic_node(type, left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}
