/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:09:36 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/13 20:07:44 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "cd") == 0
		|| ft_strcmp(cmd->args[0], "echo") == 0
		|| ft_strcmp(cmd->args[0], "pwd") == 0
		|| ft_strcmp(cmd->args[0], "export") == 0
		|| ft_strcmp(cmd->args[0], "unset") == 0
		|| ft_strcmp(cmd->args[0], "env") == 0
		|| ft_strcmp(cmd->args[0], "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (cd_exec(cmd, *env));
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (echo_exec(cmd));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (pwd_exec(*env));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (export_exec(cmd, env));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset_exec(cmd, env));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		if (cmd->ac > 1)
			return (write(2, "env: too many arguments\n", 24), 1);
		else
			return (env_exec(*env));
	}
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (exit_exec(cmd));
	return (1);
}
