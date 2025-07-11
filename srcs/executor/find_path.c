/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:48:37 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 00:01:45 by itaharbo         ###   ########.fr       */
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
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
				ft_putendl_fd(": No such file or directory", 2), NULL);
		if (is_directory(cmd))
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
				ft_putendl_fd(": Is a directory", 2), (char *)126);
		if (access(cmd, X_OK) != 0)
		{
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
				ft_putendl_fd(": Permission denied", 2), (char *)126);
		}
		tmp = ft_strdup(cmd);
		if (!tmp)
			return (ft_putendl_fd("minishell: memory allocation failed", 2),
				NULL);
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
		if (access(full_path, F_OK) == 0)
		{
			if (is_directory(full_path))
			{
				free(full_path);
				i++;
				continue ;
			}
			if (access(full_path, X_OK) == 0)
				return (ft_free_split(new_paths), full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(new_paths);
	return (NULL);
}
