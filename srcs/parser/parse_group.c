/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_group.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:00:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/07 17:01:04 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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
	return (group);
}
