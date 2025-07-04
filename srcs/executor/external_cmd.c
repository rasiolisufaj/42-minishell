/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:12:12 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/04 23:15:03 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

bool	is_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == -1)
		return (false);
	return (S_ISDIR(statbuf.st_mode));
}

char	*find_path(char *cmd, t_env *env)
{
	char	*path;
	char	*full_path;
	char	*tmp;
	char	**new_paths;
	int		i;

	if (!cmd || !env)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
			return (ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd(cmd, 2), ft_putendl_fd(
				": No such file or directory", 2),  NULL);
		if (is_directory(cmd))
			return (ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd(cmd, 2), ft_putendl_fd(
				": Is a directory", 2), NULL);
		if (access(cmd, X_OK) != 0)
		{
			return (ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd(cmd, 2), ft_putendl_fd(
				": Permission denied", 2), NULL);
		}
		tmp = ft_strdup(cmd);
		if (!tmp)
			return (ft_putendl_fd(
				"minishell: memory allocation failed", 2), NULL);
		return (tmp);
	}
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	new_paths = ft_split(path, ':');
	if (!new_paths)
		return (NULL);
	i = 0;
	while (new_paths[i])
	{
		tmp = ft_strjoin(new_paths[i], "/");
		if (!tmp)
			return (ft_free_split(new_paths), NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (ft_free_split(new_paths), NULL);
		if (access(full_path, X_OK) == 0)
			return (ft_free_split(new_paths), full_path);
		free(full_path);
		i++;
	}
	ft_free_split(new_paths);
	return (NULL);
}

int	count_env_nodes(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported)
			count++;
		env = env->next;
	}
	return (count);
}

void	free_envp_part(char **envp, int start)
{
	int	i;
	
	if (!envp)
		return;
	i = 0;
	while (i < start)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**env_to_envp(t_env *env)
{
	t_env	*current;
	char	**envp;
	char	*tmp;
	int		i;
	int		count;

	count = count_env_nodes(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	current = env;
	i = 0;
	while (current && i < count)
	{
		if (current->exported)
		{
			tmp = ft_strjoin(current->key, "=");
			if (!tmp)
				return (free_envp_part(envp, i), NULL);
			envp[i] = ft_strjoin(tmp, current->value);
			free(tmp);
			if (!envp[i])
				return (free_envp_part(envp, i), NULL);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	child_external(char *path_cmd, char **envp, t_cmd *cmd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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

	waitpid(pid, &status, 0);
	free(path_cmd);
	ft_free_split(envp);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
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
	{
		if (!ft_strchr(cmd->args[0], '/'))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
		return (127);
	}
	envp = env_to_envp(env);
	if (!envp)
		return (free(path_cmd), ft_putendl_fd(
			"minishell: memory allocation failed", 2), 1);
	pid = fork();
	if (pid < 0)
		return (free(path_cmd), ft_free_split(envp),
			ft_putendl_fd("minishell: fork failed", 2), 1);
	else if (pid == 0)
	{
		child_external(path_cmd, envp, cmd);
		exit(127);
	}
	else
		return (parent_external(pid, path_cmd, envp));
	return (0);
}
