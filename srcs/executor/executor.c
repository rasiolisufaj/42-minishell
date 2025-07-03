/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/03 19:10:12 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	exec_cmd(t_node *ast, t_env **env)
{
	t_cmd	*cmd;
	int		status;

	if (ast == NULL || ast->type != N_CMD)
		return (1);
	cmd = init_cmd(ast);
	if (!cmd)
		return (1);
	if (handle_redir(ast->redir_list) != 0)
		return (free_cmd(cmd), 1);
	if (is_builtin(cmd->args[0]))
	{
		status = exec_builtin(cmd, env);
		return (free_cmd(cmd), status);
	}
	else
		status = exec_external(cmd, *env);
	free_cmd(cmd);
	return (status);
}

int	exec_pipe(t_node *ast, t_env **env)
{
}

int	exec_and(t_node *ast, t_env **env)
{
}

int	exec_or(t_node *ast, t_env **env)
{
}

int	exec_group(t_node *ast, t_env **env)
{
}

int	execute(t_node *ast, t_env **env)
{
	if (!ast)
		return (1);
	if (ast->type == N_CMD)
		return (exec_cmd(ast, env));
	else if (ast->type == N_PIPE)
		return (exec_pipe(ast, env));
	else if (ast->type == N_AND)
		return (exec_and(ast, env));
	else if (ast->type == N_OR)
		return (exec_or(ast, env));
	else if (ast->type == N_GROUP)
		return (exec_group(ast, env));
	else
		return (ft_putstr_fd(
			"Error: Unknown AST node type ", 2), ft_putendl_fd(ast->type, 2), 1);
	return (127);
}
