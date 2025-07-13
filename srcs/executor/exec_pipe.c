/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/14 01:09:31 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	return (0);
}

static pid_t	spawn_left_child(t_node *ast, t_env **env, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		if (ast->left->redir_list && handle_redir(*env, ast, NULL) != 0)
			exit(1);
		exit(execute(ast->left, env));
	}
	return (pid);
}

static pid_t	spawn_right_child(t_node *ast, t_env **env, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		if (ast->right->redir_list && handle_redir(*env, ast, NULL) != 0)
			exit(1);
		exit(execute(ast->right, env));
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

int	exec_pipe(t_node *ast, t_env **env)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (create_pipe(pipefd) != 0)
		return (1);
	left_pid = spawn_left_child(ast, env, pipefd);
	if (left_pid < 0)
		return (1);
	right_pid = spawn_right_child(ast, env, pipefd);
	if (right_pid < 0)
		return (1);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_pipe_children(left_pid, right_pid));
}
