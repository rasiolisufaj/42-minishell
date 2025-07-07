/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:08:07 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/07 19:45:01 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
	{
		while (cmd->args[cmd->ac])
		{
			free(cmd->args[cmd->ac]);
			cmd->ac--;
		}
		free(cmd->args);
	}
	free(cmd);
}

int	count_tokens(t_token *tokens)
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

void	dup_to_tokens(t_token *tokens, char **args, int *ac)
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

int	exec_cmd(t_node *ast, t_env **env)
{
	t_cmd	*cmd;
	int		status;
	int		saved_in;
	int		saved_out;

	if (ast == NULL || ast->type != N_CMD)
		return (1);
	cmd = init_cmd(ast);
	if (!cmd)
		return (1);
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (handle_redir(ast->redir_list) != 0)
		return (free_cmd(cmd), 1);
	if (is_builtin(cmd))
		status = exec_builtin(cmd, env);
	else
		status = exec_external(cmd, *env);
	dup2(saved_in, STDIN_FILENO);
	close(saved_in);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_out);
	free_cmd(cmd);
	return (status);
}
