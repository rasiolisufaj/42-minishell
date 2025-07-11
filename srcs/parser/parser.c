/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:53:04 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/07 18:53:11 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

t_node	*parser(const char *input, t_env *env, int last_status)
{
	t_token	*tokens;
	t_token	*tokens_head;
	t_node	*ast;

	tokens = lexer(input);
	if (!tokens)
		return (NULL);
	tokens_head = tokens;
	ast = parse_and_or(&tokens);
	if (!ast)
		return (free_tokens(tokens_head), NULL);
	if (tokens)
	{
		free_node(ast);
		free_tokens(tokens_head);
		return (parser_error("syntax error: unexpected token after cmd"), NULL);
	}
	expand_ast(ast, env, last_status);
	expand_wildcards(ast);
	free_tokens(tokens_head);
	return (ast);
}
