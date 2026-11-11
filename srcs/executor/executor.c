/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/16 19:29:12 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	exec_and(t_node *ast, t_shell *sh)
{
	int	status;

	status = execute(ast->left, sh);
	if (status != 0)
		return (status);
	sh->last_status = status;
	return (execute(ast->right, sh));
}

int	exec_or(t_node *ast, t_shell *sh)
{
	int	status;

	status = execute(ast->left, sh);
	if (status == 0 || status >= EXIT_FLAG)
		return (status);
	sh->last_status = status;
	return (execute(ast->right, sh));
}

static void	group_child(t_node *ast, t_shell *sh)
{
	int	status;

	if (handle_redir(ast->redir_list) != 0)
	{
		cleanup_shell(sh->ast, sh->env);
		exit(1);
	}
	setup_signals_exec();
	status = execute(ast->left, sh);
	cleanup_shell(sh->ast, sh->env);
	exit(status & 0xFF);
}

int	exec_group(t_node *ast, t_shell *sh)
{
	int		wstatus;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		group_child(ast, sh);
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

int	execute(t_node *ast, t_shell *sh)
{
	if (!ast)
		return (0);
	if (ast->type == N_CMD)
		return (exec_cmd(ast, sh));
	if (ast->type == N_PIPE)
		return (exec_pipe(ast, sh));
	if (ast->type == N_AND)
		return (exec_and(ast, sh));
	if (ast->type == N_OR)
		return (exec_or(ast, sh));
	if (ast->type == N_GROUP)
		return (exec_group(ast, sh));
	return (127);
}
