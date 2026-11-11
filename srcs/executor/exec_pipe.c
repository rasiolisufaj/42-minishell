/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/17 14:29:24 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

// Runs one side of the pipe in a child. `end` is the fd the child replaces:
// STDOUT_FILENO for the left side, STDIN_FILENO for the right side.
static pid_t	spawn_child(t_node *node, t_shell *sh, int pipefd[2], int end)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pipefd[1 - end]);
		dup2(pipefd[end], end);
		close(pipefd[end]);
		status = execute(node, sh);
		cleanup_shell(sh->ast, sh->env);
		exit(status & 0xFF);
	}
	return (pid);
}

static int	wait_for_pipe_children(pid_t left_pid, pid_t right_pid)
{
	int	status;
	int	final_status;

	final_status = 1;
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		final_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		return (128 + WTERMSIG(status));
	}
	return (final_status);
}

int	exec_pipe(t_node *ast, t_shell *sh)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	left_pid = spawn_child(ast->left, sh, pipefd, STDOUT_FILENO);
	if (left_pid < 0)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	right_pid = spawn_child(ast->right, sh, pipefd, STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	if (right_pid < 0)
		return (waitpid(left_pid, NULL, 0), 1);
	return (wait_for_pipe_children(left_pid, right_pid));
}
