/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:48:37 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/13 21:34:54 by itaharbo         ###   ########.fr       */
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

static char	*handle_absolute_or_relative_path(char *cmd)
{
	char	*tmp;

	if (access(cmd, F_OK) != 0)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
			ft_putendl_fd(": No such file or directory", 2), NULL);
	if (is_directory(cmd))
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
			ft_putendl_fd(": Is a directory", 2), (char *)126);
	if (access(cmd, X_OK) != 0)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
			ft_putendl_fd(": Permission denied", 2), (char *)126);
	tmp = ft_strdup(cmd);
	if (!tmp)
		return (ft_putendl_fd("minishell: memory allocation failed", 2), NULL);
	return (tmp);
}

static char	*check_executable_validity(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK) == 0)
	{
		if (is_directory(full_path))
		{
			free(full_path);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
			return (full_path);
	}
	free(full_path);
	return (NULL);
}

static char	*try_search_in_path(char *cmd, t_env *env)
{
	char	*path;
	char	**new_paths;
	char	*full_path;
	int		i;

	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	new_paths = ft_split(path, ':');
	if (!new_paths)
		return (NULL);
	i = 0;
	while (new_paths[i])
	{
		full_path = check_executable_validity(new_paths[i], cmd);
		if (full_path)
		{
			ft_free_split(new_paths);
			return (full_path);
		}
		i++;
	}
	ft_free_split(new_paths);
	return (NULL);
}

char	*find_path(char *cmd, t_env *env)
{
	if (!cmd || !env)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_absolute_or_relative_path(cmd));
	return (try_search_in_path(cmd, env));
}
