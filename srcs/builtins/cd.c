/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:42:16 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/02 17:42:47 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	cd_exec(t_cmd *cmd)
{
	char	*home;
	char	*targ;

	if (cmd->ac > 2)
		return (write(2, "cd: too many arguments\n", 23), EXIT_FAILURE);
	else if (cmd->ac == 1)
	{
		home = getenv("HOME");
		if (home == NULL)
			return (write(2, "cd: HOME not set\n", 17), EXIT_FAILURE);
		targ = home;
	}
	else
		targ = cmd->args[1];
	if (chdir(targ) != 0)
	{
		write(2, "cd: ", 4);
		write(2, targ, ft_strlen(targ));
		write(2, ": No such file or directory\n", 28);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
