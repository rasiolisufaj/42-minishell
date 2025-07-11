/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:08:07 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/08 20:37:14 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
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

void	close_files(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	close(saved_in);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_out);
}

void	empty_cmd(t_cmd *cmd, int saved_in, int saved_out)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(": command not found", 2);
	close_files(saved_in, saved_out);
	free_cmd(cmd);
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
		return (close_files(saved_in, saved_out), free_cmd(cmd), 1);
	if (!cmd->args || !cmd->args[0])
		return (close_files(saved_in, saved_out), free_cmd(cmd), 0);
	if (cmd->args[0][0] == '\0')
		return (empty_cmd(cmd, saved_in, saved_out), 127);
	if (is_builtin(cmd))
		status = exec_builtin(cmd, env);
	else
		status = exec_external(cmd, *env);
	close_files(saved_in, saved_out);
	free_cmd(cmd);
	return (status);
}
