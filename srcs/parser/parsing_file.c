/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:27 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/19 22:05:57 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

bool	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC);
}

static void	append_token(t_token **head, t_token **tail, t_token *src)
{
	t_token	*n;

	n = malloc(sizeof(t_token));
	if (!n)
		parser_error("malloc failed in append_token");
	n->type = src->type;
	n->value = ft_strdup(src->value);
	if (!n->value)
	{
		free(n);
		parser_error("strdup failed in append_token");
	}
	n->next = NULL;
	if (!*head)
		*head = n;
	else
		(*tail)->next = n;
	*tail = n;
}

t_node	*create_node(t_node_type ope)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
	{
		fprintf(stderr, "Error: Memory allocation failed\n");
		return (NULL);
	}
	new_node->type = ope;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->tokens = NULL;
	new_node->redir_list = NULL;
	return (new_node);
}

void	parser_error(const char *msg)
{
	fprintf(stderr, "minishell: %s\n", msg);
	exit(EXIT_FAILURE);
}

t_token	*dup_token_range(t_token *start, t_token *end)
{
	t_token	*new_token;
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	if (!start)
		return (NULL);
	while (start != end)
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return (fprintf(stderr, "Malloc has failed\n"),
				free_tokens(head), NULL);
		new_token->type = start->type;
		new_token->value = ft_strdup(start->value);
		if (!new_token->value)
		{
			fprintf(stderr,
				"Error: memory allocation failed for token value\n");
			return (free(new_token), free_tokens(head), NULL);
		}
		new_token->next = NULL;
		if (!head)
			head = new_token;
		else
			tail->next = new_token;
		tail = new_token;
		start = start->next;
	}
	return (head);
}

t_node	*parse_pipe(t_token **tokens)
{
	return (parse_cmd(tokens));
}

t_node	*parse_and_or(t_token **tokens)
{
	t_node			*left;
	t_node			*right;
	t_node			*node;
	t_token_type	type;

	if (!tokens)
		return (NULL);
	left = parse_pipe(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->type == T_AND || (*tokens)->type == T_OR))
	{
		type = (*tokens)->type;
		(*tokens) = (*tokens)->next;
		right = parse_pipe(tokens);
		if (!right)
		{
			free_node(left);
			parser_error("syntax error: expected command after '&&' or '||'");
			return (NULL);
		}
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
		left = node;
	}
	return (left);
}

t_node	*parse_group(t_token **tokens)
{
	t_node	*group;

	if (!tokens || !(*tokens) || (*tokens)->type != T_PAREN_OPEN)
		parser_error("syntax error: expected '('");
	(*tokens) = (*tokens)->next;
	group = create_node(N_GROUP);
	if (!group)
		return (NULL);
	group->left = parse_and_or(tokens);
	if (!group->left)
		return (free(group), NULL);
	if (!(*tokens) || (*tokens)->type != T_PAREN_CLOSE)
	{
//		free_node(group);
//		free(group);
		parser_error("syntax error: expected ')'");
	}
	(*tokens) = (*tokens)->next;
	return (group);
}

t_node	*parse_cmd(t_token **tokens)
{
//	t_token	*start;
	t_token	*args_head;
	t_token	*args_tail;
	t_token	*redir_head;
	t_token	*redir_tail;
	t_node	*node;
	
	if (!tokens || !*tokens)
		return (NULL);
	args_head = NULL;
	args_tail = NULL;
	redir_head = NULL;
	redir_tail = NULL;
	if (*tokens && (*tokens)->type == T_PAREN_OPEN)
		return (parse_group(tokens));
//	start = *tokens;
	while (*tokens && ((*tokens)->type == T_WORD
			|| is_redirection((*tokens)->type)))
	{
		if (is_redirection((*tokens)->type))
		{
			append_token(&redir_head, &redir_tail, *tokens);
			(*tokens) = (*tokens)->next;
			if ((*tokens) && (*tokens)->type == T_WORD)
			{
				append_token(&redir_head, &redir_tail, *tokens);
				(*tokens) = (*tokens)->next;
			}
			else
				parser_error("syntax error after redirection");
		}
		else
		{
			append_token(&args_head, &args_tail, *tokens);
			*tokens = (*tokens)->next;
		}
	}
	node = create_node(N_CMD);
	if (!node)
		return (NULL);
	node->tokens = args_head;
	node->redir_list = redir_head;
	return (node);
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	free_tokens(node->tokens);
	free_tokens(node->redir_list);
	free(node);
}

/*t_node	*parsing_file(const char *input)
{
	t_token	*tokens;
	t_token	*current;
	t_node	*root;

	tokens = lexer(input);
	if (!tokens)
	{
		fprintf(stderr, "Error: Lexer failed\n");
		return (NULL);
	}
	current = tokens;
	root = NULL;
	while (current)
	{
		current = current->next;
	}
	free_tokens(tokens);
	return (root);
}*/
