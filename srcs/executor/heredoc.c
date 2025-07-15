/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:07:56 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/15 21:05:30 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static void	free_cmd(t_cmd *cmd)
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

static bool	child_readline(char *stop, int pipe_fd[2])
{
	char	*line;

	line = readline("> ");
	if (!line || ft_strcmp(line, stop) == 0)
	{
		if (line)
			free(line);
		return (FALSE);
	}
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
	free(line);
	return (TRUE);
}

void	heredoc_child(int pipe_fd[2], t_env *env, t_node *ast,
		t_heredoc_args *args)
{
	char		*stop;

	close(args->saved_in);
	close(args->saved_out);
	stop = ft_strdup(args->delimiter);
	if (!stop)
	{
		ft_putendl_fd("Error: Memory allocation failed", 2);
		exit(1);
	}
	close(pipe_fd[0]);
	free_cmd(args->cmd);
	cleanup_shell(ast, env);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		if (!child_readline(stop, pipe_fd))
			break ;
	}
	free(stop);
	close(pipe_fd[1]);
	exit(0);
}

void	sig_heredoc(int status, int *sig)
{
	*sig = WTERMSIG(status);
	if (*sig == SIGINT)
		ft_putstr_fd("\n", 2);
}

int	handle_heredoc(t_env *env, t_node *ast, t_heredoc_args *args)
{
	int				pipe_fd[2];
	int				status;
	int				sig;
	pid_t			pid;

	if (pipe(pipe_fd) < 0)
		return (perror("minishell: pipe"), 1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), signal(SIGINT, sigint_handler),
			close(pipe_fd[0]), close(pipe_fd[1]), 1);
	if (pid == 0)
		heredoc_child(pipe_fd, env, ast, args);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	if (WIFSIGNALED(status))
		return (sig_heredoc(status, &sig), close(pipe_fd[0]), 128 + sig);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		return (perror("minishell: dup2"), close(pipe_fd[0]), 1);
	return (close(pipe_fd[0]), 0);
}
