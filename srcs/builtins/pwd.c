/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:43:53 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/13 20:13:34 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	pwd_exec(t_env *env)
{
	char	*path;
	char	*pwd_env;

	path = getcwd(NULL, 0);
	if (path)
		return (ft_putendl_fd(path, 1), free(path), EXIT_SUCCESS);
	pwd_env = get_env_value(env, "PWD");
	if (pwd_env && *pwd_env)
	{
		ft_putendl_fd(pwd_env, 1);
		return (EXIT_SUCCESS);
	}
	ft_putstr_fd("minishell: pwd: error retrieving current directory: ", 2);
	perror("getcwd");
	return (EXIT_FAILURE);
}
