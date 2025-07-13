/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:59:19 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/13 21:14:12 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

bool	validate_path_exists(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory\n", 2);
		return (false);
	}
	if (!S_ISDIR(statbuf.st_mode))
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Not a directory\n", 2);
		return (false);
	}
	return (true);
}

bool	change_to_directory(char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (false);
	}
	return (true);
}

void	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (ft_putstr_fd(
						"minishell: cd: memory allocation failed\n", 2));
			return ;
		}
		current = current->next;
	}
	current = malloc(sizeof(t_env));
	if (!current)
		return (ft_putstr_fd("minishell: cd: memory allocation failed\n", 2));
	current->key = ft_strdup(key);
	current->value = ft_strdup(value);
	current->exported = true;
	current->next = *env;
	*env = current;
}
