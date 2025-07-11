/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:43:01 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/02 17:43:32 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

bool	is_n_flag(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] != '-')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	echo_exec(t_cmd *cmd)
{
	int		j;
	bool	n_flag;

	if (cmd->ac < 2)
		return (write(1, "\n", 1), EXIT_SUCCESS);
	n_flag = false;
	j = 1;
	if (is_n_flag(cmd->args[1]))
	{
		n_flag = true;
		j = 2;
		while (cmd->args[j] && is_n_flag(cmd->args[j]))
			j++;
	}
	while (cmd->args[j])
	{
		write(1, cmd->args[j], ft_strlen(cmd->args[j]));
		if (j + 1 < cmd->ac)
			write(1, " ", 1);
		j++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
