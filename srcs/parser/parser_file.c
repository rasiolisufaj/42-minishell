/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:27 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/07 16:10:26 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

// void	parser_error(char *msg)
// {
// 	ft_putstr_fd("minishell: ", 2);
// 	ft_putendl_fd(msg, 2);
// }

// bool	is_redirection(t_token_type type)
// {
// 	return (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_REDIR_APPEND
// 		|| type == T_HEREDOC);
// }

// void	append_token(t_token **head, t_token **tail, t_token *src)
// {
// 	t_token	*n;

// 	n = malloc(sizeof(t_token));
// 	if (!n)
// 	{
// 		parser_error("malloc failed in append_token");
// 		return ;
// 	}
// 	n->type = src->type;
// 	n->value = ft_strdup(src->value);
// 	if (!n->value)
// 	{
// 		free(n);
// 		parser_error("strdup failed in append_token");
// 		return ;
// 	}
// 	n->next = NULL;
// 	if (!*head)
// 		*head = n;
// 	else
// 		(*tail)->next = n;
// 	*tail = n;
// }

// t_node	*create_node(t_node_type type)
// {
// 	t_node	*new_node;

// 	new_node = malloc(sizeof(t_node));
// 	if (!new_node)
// 		return (ft_putendl_fd("Error: Memory allocation failed", 2), NULL);
// 	new_node->type = type;
// 	new_node->left = NULL;
// 	new_node->right = NULL;
// 	new_node->tokens = NULL;
// 	new_node->redir_list = NULL;
// 	return (new_node);
// }

// t_node	*parse_pipe(t_token **tokens)
// {
// 	t_node	*left;
// 	t_node	*right;
// 	t_node	*pipe_node;

// 	if (!tokens || !(*tokens))
// 		return (NULL);
// 	if ((*tokens)->type == T_PIPE)
// 		return (parser_error("syntax error near unexpected token '|'"), NULL);
// 	left = parse_cmd(tokens);
// 	if (!left)
// 		return (NULL);
// 	while (*tokens && (*tokens)->type == T_PIPE)
// 	{
// 		(*tokens) = (*tokens)->next;
// 		if (!(*tokens) || (*tokens)->type == T_PIPE)
// 		{
// 			free_node(left);
// 			return (parser_error("syntax error near unexpected token '|'"),
// 				NULL);
// 		}
// 		right = parse_cmd(tokens);
// 		if (!right)
// 		{
// 			free_node(left);
// 			return (parser_error("syntax error near unexpected token '|'"),
// 				NULL);
// 		}
// 		pipe_node = create_node(N_PIPE);
// 		if (!pipe_node)
// 		{
// 			free_node(left);
// 			free_node(right);
// 			return (NULL);
// 		}
// 		pipe_node->left = left;
// 		pipe_node->right = right;
// 		left = pipe_node;
// 	}
// 	return (left);
// }

// t_node	*parse_and_or(t_token **tokens)
// {
// 	t_node			*left;
// 	t_node			*right;
// 	t_node			*node;
// 	t_token_type	type;

// 	if (!tokens || !(*tokens))
// 		return (NULL);
// 	if ((*tokens)->type == T_AND || (*tokens)->type == T_OR)
// 	{
// 		if ((*tokens)->type == T_AND)
// 			parser_error("syntax error near unexpected token '&&'");
// 		else
// 			parser_error("syntax error near unexpected token '||'");
// 		return (NULL);
// 	}
// 	left = parse_pipe(tokens);
// 	if (!left)
// 		return (NULL);
// 	while (*tokens && ((*tokens)->type == T_AND || (*tokens)->type == T_OR))
// 	{
// 		type = (*tokens)->type;
// 		(*tokens) = (*tokens)->next;
// 		if (!*tokens || (*tokens)->type == T_AND || (*tokens)->type == T_OR)
// 		{
// 			free_node(left);
// 			if (type == T_AND)
// 				parser_error("syntax error near unexpected token '&&'");
// 			else
// 				parser_error("syntax error near unexpected token '||'");
// 			return (NULL);
// 		}
// 		right = parse_pipe(tokens);
// 		if (!right)
// 		{
// 			free_node(left);
// 			if (type == T_AND)
// 				parser_error("syntax error near unexpected token '&&'");
// 			else
// 				parser_error("syntax error near unexpected token '||'");
// 			return (NULL);
// 		}
// 		if (type == T_AND)
// 			node = create_node(N_AND);
// 		else
// 			node = create_node(N_OR);
// 		if (!node)
// 		{
// 			free_node(left);
// 			free_node(right);
// 			return (NULL);
// 		}
// 		node->left = left;
// 		node->right = right;
// 		left = node;
// 	}
// 	return (left);
// }

// t_node	*parse_group(t_token **tokens)
// {
// 	t_node	*group;

// 	if (!tokens || !(*tokens) || (*tokens)->type != T_PAREN_OPEN)
// 		return (parser_error("syntax error: expected '('"), NULL);
// 	(*tokens) = (*tokens)->next;
// 	if (!(*tokens) || (*tokens)->type == T_PAREN_CLOSE)
// 		return (parser_error("syntax error: unexpected ')'"), NULL);
// 	group = create_node(N_GROUP);
// 	if (!group)
// 		return (NULL);
// 	group->left = parse_and_or(tokens);
// 	if (!group->left)
// 		return (free(group), NULL);
// 	if (!(*tokens) || (*tokens)->type != T_PAREN_CLOSE)
// 	{
// 		free_node(group);
// 		return (parser_error("syntax error: expected ')'"), NULL);
// 	}
// 	(*tokens) = (*tokens)->next;
// 	return (group);
// }

// t_node	*parse_cmd(t_token **tokens)
// {
// 	t_token	*args_head;
// 	t_token	*args_tail;
// 	t_token	*redir_head;
// 	t_token	*redir_tail;
// 	t_node	*node;

// 	if (!tokens || !*tokens)
// 		return (NULL);
// 	args_head = NULL;
// 	args_tail = NULL;
// 	redir_head = NULL;
// 	redir_tail = NULL;
// 	if (*tokens && (*tokens)->type == T_PAREN_OPEN)
// 		return (parse_group(tokens));
// 	while (*tokens && ((*tokens)->type == T_WORD
// 			|| is_redirection((*tokens)->type)))
// 	{
// 		if (is_redirection((*tokens)->type))
// 		{
// 			append_token(&redir_head, &redir_tail, *tokens);
// 			(*tokens) = (*tokens)->next;
// 			if ((*tokens) && (*tokens)->type == T_WORD)
// 			{
// 				append_token(&redir_head, &redir_tail, *tokens);
// 				(*tokens) = (*tokens)->next;
// 			}
// 			else
// 			{
// 				free_tokens(args_head);
// 				free_tokens(redir_head);
// 				return (parser_error("syntax error after redirection"), NULL);
// 			}
// 		}
// 		else
// 		{
// 			append_token(&args_head, &args_tail, *tokens);
// 			*tokens = (*tokens)->next;
// 		}
// 	}
// 	node = create_node(N_CMD);
// 	if (!node)
// 		return (NULL);
// 	node->tokens = args_head;
// 	node->redir_list = redir_head;
// 	return (node);
// }

// void	free_node(t_node *node)
// {
// 	if (!node)
// 		return ;
// 	free_node(node->left);
// 	free_node(node->right);
// 	free_tokens(node->tokens);
// 	free_tokens(node->redir_list);
// 	free(node);
// }

// t_node	*parser(const char *input, t_env *env, int last_status)
// {
// 	t_token	*tokens;
// 	t_token	*tokens_head;
// 	t_node	*ast;

// 	tokens = lexer(input);
// 	if (!tokens)
// 		return (NULL);
// 	tokens_head = tokens;
// 	ast = parse_and_or(&tokens);
// 	if (!ast)
// 		return (free_tokens(tokens_head), NULL);
// 	if (tokens)
// 	{
// 		free_node(ast);
// 		free_tokens(tokens_head);
// 		return (parser_error("syntax error: unexpected token after cmd"), NULL);
// 	}
// 	expand_ast(ast, env, last_status);
// 	expand_wildcards(ast);
// 	free_tokens(tokens_head);
// 	return (ast);
// }
