/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:07:56 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/17 17:07:22 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static void	write_heredoc_body(int fd, char *delim, bool expand, t_shell *sh)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by "
				"end-of-file (wanted `", 2);
			ft_putstr_fd(delim, 2);
			ft_putendl_fd("')", 2);
			return ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			return ;
		}
		if (expand)
			line = expand_heredoc_line(line, sh);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static void	heredoc_child(int fd, char *raw_delim, t_shell *sh)
{
	char	*delim;
	bool	expand;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	expand = !check_quotes(raw_delim);
	delim = strip_quotes(raw_delim);
	if (delim)
		write_heredoc_body(fd, delim, expand, sh);
	free(delim);
	close(fd);
	cleanup_shell(sh->ast, sh->env);
	exit(0);
}

// Reads one heredoc into a temp file and makes `word` point to that file.
// Returns 0, or the status to report: 1 on error, 130 on ctrl-C.
static int	read_one_heredoc(t_token *word, t_shell *sh)
{
	char	*path;
	int		fd;
	int		status;
	pid_t	pid;

	path = heredoc_path(&fd);
	if (!path)
		return (perror("minishell: heredoc"), 1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), close(fd), unlink(path),
			free(path), setup_signals(), 1);
	if (pid == 0)
		heredoc_child(fd, word->value, sh);
	close(fd);
	free(word->value);
	word->value = path;
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
		return (write(1, "\n", 1), 130);
	return (0);
}

// Reads every heredoc of the line, in order, before anything runs.
int	read_heredocs(t_node *node, t_shell *sh)
{
	t_token	*redir;
	int		ret;

	if (!node)
		return (0);
	ret = read_heredocs(node->left, sh);
	if (ret == 0)
		ret = read_heredocs(node->right, sh);
	redir = node->redir_list;
	while (ret == 0 && redir)
	{
		if (redir->type == T_HEREDOC && redir->next)
			ret = read_one_heredoc(redir->next, sh);
		redir = redir->next;
	}
	return (ret);
}

// Only paths we created are removed: after a ctrl-C, later heredocs still
// hold their delimiter, which may be the name of a real file.
void	unlink_heredocs(t_node *node)
{
	t_token	*redir;

	if (!node)
		return ;
	unlink_heredocs(node->left);
	unlink_heredocs(node->right);
	redir = node->redir_list;
	while (redir)
	{
		if (redir->type == T_HEREDOC && redir->next
			&& ft_strncmp(redir->next->value, HEREDOC_PREFIX,
				ft_strlen(HEREDOC_PREFIX)) == 0)
			unlink(redir->next->value);
		redir = redir->next;
	}
}
