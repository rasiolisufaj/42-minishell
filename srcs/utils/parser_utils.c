/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:21:20 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/07 16:23:29 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	parser_error(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(msg, 2);
}

bool	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_REDIR_APPEND
		|| type == T_HEREDOC);
}

void	append_token(t_token **head, t_token **tail, t_token *src)
{
	t_token	*n;

	n = malloc(sizeof(t_token));
	if (!n)
	{
		parser_error("malloc failed in append_token");
		return ;
	}
	n->type = src->type;
	n->value = ft_strdup(src->value);
	if (!n->value)
	{
		free(n);
		parser_error("strdup failed in append_token");
		return ;
	}
	n->next = NULL;
	if (!*head)
		*head = n;
	else
		(*tail)->next = n;
	*tail = n;
}

t_node	*create_node(t_node_type type)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (ft_putendl_fd("Error: Memory allocation failed", 2), NULL);
	new_node->type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->tokens = NULL;
	new_node->redir_list = NULL;
	return (new_node);
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	free_node(node->left);
	free_node(node->right);
	free_tokens(node->tokens);
	free_tokens(node->redir_list);
	free(node);
}
