/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:12:12 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 17:06:09 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	child_external(char *path_cmd, char **envp, t_cmd *cmd)
{
	setup_signals_exec();
	if (execve(path_cmd, cmd->args, envp) == -1)
	{
		ft_putstr_fd("minishell: execve failed: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		free(path_cmd);
		ft_free_split(envp);
		exit(127);
	}
}

int	parent_external(pid_t pid, char *path_cmd, char **envp)
{
	int	status;

	setup_signals_waiting();
	waitpid(pid, &status, WUNTRACED);
	if (WIFSTOPPED(status))
	{
		printf("\n");
		setup_signals();
		free(path_cmd);
		ft_free_split(envp);
		return (130);
	}
	setup_signals();
	free(path_cmd);
	ft_free_split(envp);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

void	wrong_path_cmd(char *cmd)
{
	if (!ft_strchr(cmd, '/'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
	}
}

int	exec_external(t_cmd *cmd, t_env *env)
{
	char	*path_cmd;
	char	**envp;
	pid_t	pid;

	if (!cmd || !cmd->args || !cmd->args[0] || !env)
		return (1);
	path_cmd = find_path(cmd->args[0], env);
	if (!path_cmd)
		return (wrong_path_cmd(cmd->args[0]), 127);
	if (path_cmd == (char *)126)
		return (126);
	envp = env_to_envp(env);
	if (!envp)
		return (free(path_cmd),
			ft_putendl_fd("minishell: memory allocation failed", 2), 1);
	pid = fork();
	if (pid < 0)
		return (free(path_cmd), ft_free_split(envp),
			ft_putendl_fd("minishell: fork failed", 2), 1);
	else if (pid == 0)
	{
		child_external(path_cmd, envp, cmd);
		exit(127);
	}
	return (parent_external(pid, path_cmd, envp));
}
