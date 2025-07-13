/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:05:19 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/13 14:07:05 by itaharbo         ###   ########.fr       */
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

int	handle_redir(t_env *env, t_node *ast, t_cmd *cmd)
{
	t_token	*current;
	int		fd;

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
		else if (current->type == T_HEREDOC)
		{
			if (handle_heredoc(current->next->value, env, ast, cmd) != 0)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
