/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:42:16 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 20:56:46 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"
#include <sys/stat.h>

static void	check_current_directory(char *path)
{
	char	*cwd;

	if (path[0] != '/')
	{
		cwd = getcwd(NULL, 0);
		if (cwd == NULL)
		{
			ft_putstr_fd("cd: error retrieving current directory: ", 2);
			perror("getcwd");
		}
		else
			free(cwd);
	}
}

static bool	validate_path_exists(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 28);
		return (false);
	}
	if (!S_ISDIR(statbuf.st_mode))
	{
		ft_putstr_fd("minishell: cd: ", 2);
		write(2, path, ft_strlen(path));
		write(2, ": Not a directory\n", 18);
		return (false);
	}
	return (true);
}

static bool	change_to_directory(char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		write(2, path, ft_strlen(path));
		write(2, ": Permission denied\n", 20);
		return (false);
	}
	return (true);
}

static bool	cd_change_directory(char *path)
{
	check_current_directory(path);
	if (!validate_path_exists(path))
		return (false);
	if (!change_to_directory(path))
		return (false);
	return (true);
}

int	cd_exec(t_cmd *cmd, t_env *env)
{
	char	*home;
	char	*targ;

	if (cmd->ac > 2)
		return (ft_putendl_fd("minishell: cd: too many arguments", 2),
			EXIT_FAILURE);
	else if (cmd->ac == 1 || (cmd->ac == 2 && cmd->args[1][0] == '~'))
	{
		home = get_env_value(env, "HOME");
		if (home == NULL)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2),
				EXIT_FAILURE);
		targ = home;
	}
	else
		targ = cmd->args[1];
	if (!cd_change_directory(targ))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
