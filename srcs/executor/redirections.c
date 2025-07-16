/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:05:19 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/16 16:21:10 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	redir_error(t_token *redir_list)
{
	t_token	*current;

	current = redir_list;
	while (current)
	{
		if (current->type == T_REDIR_IN || current->type == T_REDIR_OUT
			|| current->type == T_REDIR_APPEND || current->type == T_HEREDOC)
		{
			if (!current->next || current->next->type != T_WORD)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `",
					2);
				if (!current->next)
					ft_putstr_fd("newline", 2);
				else
					ft_putstr_fd(current->next->value, 2);
				ft_putendl_fd("'", 2);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	redir_opening(int *fd, t_token *current)
{
	if (current->type == T_REDIR_OUT)
	{
		*fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*fd < 0)
			return (perror("minishell: open"), 1);
		if (dup2(*fd, STDOUT_FILENO) < 0)
			return (perror("minishell: dup2"), close(*fd), 1);
	}
	else if (current->type == T_REDIR_APPEND)
	{
		*fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*fd < 0)
			return (perror("minishell: open"), 1);
		if (dup2(*fd, STDOUT_FILENO) < 0)
			return (perror("minishell: dup2"), close(*fd), 1);
	}
	else if (current->type == T_REDIR_IN)
	{
		*fd = open(current->next->value, O_RDONLY);
		if (*fd < 0)
			return (perror("minishell: open"), 1);
		if (dup2(*fd, STDIN_FILENO) < 0)
			return (perror("minishell: dup2"), close(*fd), 1);
	}
	return (close(*fd), 0);
}

t_token	*find_last_heredoc(t_token *redir_list)
{
	t_token	*last;
	t_token	*current;

	last = NULL;
	current = redir_list;
	while (current)
	{
		if (current->type == T_HEREDOC)
			last = current;
		current = current->next;
	}
	return (last);
}

bool	heredoc_redir(t_token *current, t_env *env, t_node *ast,
		t_heredoc_args *args)
{
	t_token	*last_heredoc;

	last_heredoc = find_last_heredoc(ast->redir_list);
	if (current->type == T_HEREDOC)
	{
		if (!current->next)
			return (false);
		args->delimiter = current->next->value;
		if (current == last_heredoc)
		{
			if (handle_heredoc(env, ast, args) != 0)
				return (false);
		}
		else
			handle_mult_heredoc(env, ast, args);
	}
	return (true);
}

int	handle_redir(t_env *env, t_node *ast, t_heredoc_args *args)
{
	t_token			*current;
	int				fd;

	if (!ast->redir_list)
		return (0);
	if (redir_error(ast->redir_list))
		return (1);
	current = ast->redir_list;
	while (current)
	{
		if (current->type == T_REDIR_OUT || current->type == T_REDIR_APPEND
			|| current->type == T_REDIR_IN)
		{
			if (redir_opening(&fd, current) != 0)
				return (1);
		}
		if (!heredoc_redir(current, env, ast, args))
			return (1);
		current = current->next;
	}
	return (0);
}
