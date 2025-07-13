/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:42:16 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/13 21:15:30 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	if_relative_path(char *path)
{
	char	*cwd;

	if (path[0] != '/')
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			perror("cd: error retrieving current directory: "
				"getcwd: cannot access parent directories");
		else
			free(cwd);
	}
}

char	*get_oldpwd_char(t_env *env)
{
	char	*oldpwd;
	char	*pwd_env;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		pwd_env = get_env_value(env, "PWD");
		if (pwd_env == NULL)
			oldpwd = ft_strdup("");
		else
			oldpwd = ft_strdup(pwd_env);
		if (!oldpwd)
			return (perror("malloc failed"), NULL);
	}
	return (oldpwd);
}

char	*get_newpwd_char(char *oldpwd, char *path)
{
	char	*newpwd;
	char	*tmp;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		tmp = ft_strjoin(oldpwd, "/");
		if (!tmp)
			return (perror("malloc failed"), NULL);
		newpwd = ft_strjoin(tmp, path);
		free(tmp);
		if (!newpwd)
			return (perror("malloc failed"), NULL);
	}
	return (newpwd);
}

static bool	cd_change_directory(char *path, t_env *env)
{
	char	*oldpwd;
	char	*newpwd;

	if_relative_path(path);
	oldpwd = get_oldpwd_char(env);
	if (!oldpwd)
		return (false);
	if (!validate_path_exists(path) || !change_to_directory(path))
		return (free(oldpwd), false);
	set_env_value(&env, "OLDPWD", oldpwd);
	newpwd = get_newpwd_char(oldpwd, path);
	if (!newpwd)
		return (free(oldpwd), false);
	set_env_value(&env, "PWD", newpwd);
	return (free(oldpwd), free(newpwd), true);
}

int	cd_exec(t_cmd *cmd, t_env *env)
{
	char	*targ;
	char	*home;

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
	if (!cd_change_directory(targ, env))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
