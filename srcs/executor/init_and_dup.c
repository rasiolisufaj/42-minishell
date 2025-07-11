/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_dup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:44:54 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/11 22:47:33 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

static void	dup_to_tokens(t_token *tokens, char **args, int *ac)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == T_WORD)
		{
			args[*ac] = ft_strdup(current->value);
			if (!args[*ac])
			{
				while (*ac > 0)
					free(args[--(*ac)]);
				args[0] = NULL;
				return ;
			}
			(*ac)++;
		}
		current = current->next;
	}
	args[*ac] = NULL;
}

t_cmd	*init_cmd(t_node *ast)
{
	t_cmd	*cmd;
	t_token	*current;
	int		tokens_count;

	if (!ast || ast->type != N_CMD)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	tokens_count = count_tokens(ast->tokens);
	current = ast->tokens;
	cmd->args = malloc(sizeof(char *) * (tokens_count + 1));
	if (!cmd->args)
		return (free(cmd), NULL);
	current = ast->tokens;
	cmd->ac = 0;
	dup_to_tokens(current, cmd->args, &cmd->ac);
	return (cmd);
}
