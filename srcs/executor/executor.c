/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/13 21:33:03 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	exec_and(t_node *ast, t_env **env)
{
	int	status;

	status = execute(ast->left, env);
	if (status == 0)
		return (execute(ast->right, env));
	return (status);
}

int	exec_or(t_node *ast, t_env **env)
{
	int	status;

	status = execute(ast->left, env);
	if (status != 0)
		return (execute(ast->right, env));
	return (status);
}

int	exec_group(t_node *ast, t_env **env)
{
	int		status;
	int		wstatus;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		setup_signals_exec();
		if (ast->redir_list && handle_redir(*env, ast, NULL) != 0)
			exit(1);
		status = execute(ast->left, env);
		exit(status);
	}
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
	{
		if (WTERMSIG(wstatus) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		return (128 + WTERMSIG(wstatus));
	}
	return (1);
}

int	execute(t_node *ast, t_env **env)
{
	if (!ast)
		return (0);
	if (ast->type == N_CMD)
		return (exec_cmd(ast, env));
	if (ast->type == N_PIPE)
		return (exec_pipe(ast, env));
	if (ast->type == N_AND)
		return (exec_and(ast, env));
	if (ast->type == N_OR)
		return (exec_or(ast, env));
	if (ast->type == N_GROUP)
		return (exec_group(ast, env));
	return (127);
}
