/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:07:56 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/03 19:39:02 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	heredoc_child(int pipe_fd[2], char *delimiter)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

void	sig_heredoc(int status, int *sig)
{
	*sig = WTERMSIG(status);
	if (*sig == SIGINT)
		ft_putstr_fd("\n", 2);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	int		status;
	int		sig;
	pid_t	pid;

	if (pipe(pipe_fd) < 0)
		return (perror("minishell: pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), close(pipe_fd[0]),
			close(pipe_fd[1]), 1);
	if (pid == 0)
		heredoc_child(pipe_fd, delimiter);
	else
	{
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
			return (sig_heredoc(status, &sig), close(pipe_fd[0]), 128 + sig);
		if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
			return (perror("minishell: dup2"), close(pipe_fd[0]), 1);
		close(pipe_fd[0]);
	}
	return (0);
}
