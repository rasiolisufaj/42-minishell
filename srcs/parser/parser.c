/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:53:04 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/11 23:22:34 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

// Builds the tree only. Heredocs are read and words expanded by the caller.
t_node	*parser(const char *input)
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
	free_tokens(tokens_head);
	return (ast);
}
