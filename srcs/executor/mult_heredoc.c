/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mult_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:27:58 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/15 21:05:36 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	count_heredocs(t_token *redir_list)
{
	int		count;
	t_token	*current;

	current = redir_list;
	count = 0;
	while (current)
	{
		if (current->type == T_HEREDOC)
			count++;
		current = current->next;
	}
	return (count);
}

void	read_pipe(int pipe_fd[2])
{
	char	buf[1024];
	ssize_t	n;

	n = read(pipe_fd[0], buf, sizeof(buf));
	while (n > 0)
		n = read(pipe_fd[0], buf, sizeof(buf));
	close(pipe_fd[0]);
}

int	handle_mult_heredoc(t_env *env, t_node *ast, t_heredoc_args *args)
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
	read_pipe(pipe_fd);
	return (close(pipe_fd[0]), 0);
}
