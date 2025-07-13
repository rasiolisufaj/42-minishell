/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:08:07 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/13 14:06:16 by itaharbo         ###   ########.fr       */
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
	if (handle_redir(*env, ast, cmd) != 0)
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
