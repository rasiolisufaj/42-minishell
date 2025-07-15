/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:08:07 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/15 21:00:02 by itaharbo         ###   ########.fr       */
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

void	close_files(t_heredoc_args *args)
{
	dup2(args->saved_in, STDIN_FILENO);
	close(args->saved_in);
	dup2(args->saved_out, STDOUT_FILENO);
	close(args->saved_out);
}

void	empty_cmd(t_heredoc_args *args)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(": command not found", 2);
	close_files(args);
	free_cmd(args->cmd);
}

int	exec_cmd(t_node *ast, t_env **env)
{
	int				status;
	t_heredoc_args	args;

	if (ast == NULL || ast->type != N_CMD)
		return (1);
	args.cmd = init_cmd(ast);
	if (!args.cmd)
		return (1);
	args.delimiter = NULL;
	args.saved_in = dup(STDIN_FILENO);
	args.saved_out = dup(STDOUT_FILENO);
	if (handle_redir(*env, ast, &args) != 0)
		return (close_files(&args), free_cmd(args.cmd), 1);
	if (!args.cmd->args || !args.cmd->args[0])
		return (close_files(&args), free_cmd(args.cmd), 0);
	if (args.cmd->args[0][0] == '\0')
		return (empty_cmd(&args), 127);
	if (is_builtin(args.cmd))
		status = exec_builtin(args.cmd, env);
	else
		status = exec_external(args.cmd, *env);
	close_files(&args);
	free_cmd(args.cmd);
	return (status);
}
