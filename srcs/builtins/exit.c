/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:25:12 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/07 17:47:33 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	is_numeric_argument(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] == ' ')
		i++;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i] && ft_isdigit(arg[i]))
		i++;
	while (arg[i] && arg[i] == ' ')
		i++;
	if (arg[i] != '\0')
	{
		write(2, "exit: ", 6);
		write(2, arg, ft_strlen(arg));
		write(2, ": numeric argument required\n", 28);
		return (1);
	}
	return (0);
}

int	exit_exec(t_cmd *cmd, t_env **env)
{
	int	last_status;

	last_status = 0;
	if (cmd->ac > 2)
		return (write(2, "exit: too many arguments\n", 25), 1);
	else if (cmd->ac == 2)
	{
		if (is_numeric_argument(cmd->args[1]) != 0)
		{
			free_env(*env);
			rl_clear_history();
			exit(2);
		}
		last_status = ft_atoi(cmd->args[1]) & 0xFF;
	}
	else
		last_status = g_signal & 0xFF;
	write(1, "exit\n", 5);
	free_env(*env);
	rl_clear_history();
	exit(last_status);
}
