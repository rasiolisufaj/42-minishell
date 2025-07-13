/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:25:12 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/13 18:25:40 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"
#define LLONG_MAX_STR "9223372036854775807"
#define LLONG_MIN_STR "9223372036854775808"

static void	invalid_exit_argument(char *arg)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

static void	plus_minus_sign(char *arg, int *i, int *sign)
{
	if (arg[*i] == '-' || arg[*i] == '+')
	{
		if (arg[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

int	is_numeric_argument(char *arg)
{
	int	i;
	int	sign;
	int	start;

	i = 0;
	while (arg[i] == ' ')
		i++;
	sign = 1;
	plus_minus_sign(arg, &i, &sign);
	start = i;
	while (arg[i] && ft_isdigit(arg[i]))
		i++;
	if ((i - start) == 0)
		return (invalid_exit_argument(arg), 1);
	if ((i - start) > 19 || ((i - start) == 19 && sign == 1
			&& ft_strncmp(arg + start, LLONG_MAX_STR, 19) > 0)
		|| ((i - start) == 19 && sign == -1
			&& ft_strncmp(arg + start, LLONG_MIN_STR, 19) > 0))
		return (invalid_exit_argument(arg), 1);
	while (arg[i] == ' ')
		i++;
	if (arg[i] != '\0')
		return (invalid_exit_argument(arg), 1);
	return (0);
}

int	exit_exec(t_cmd *cmd)
{
	int	exit_status;

	exit_status = 0;
	if (cmd->ac >= 2)
	{
		if (is_numeric_argument(cmd->args[1]) != 0)
			exit_status = 2;
		else if (cmd->ac > 2)
			return (ft_putstr_fd("exit\n", 1),
				write(2, "exit: too many arguments\n", 25), 1);
		else
			exit_status = ft_atoi(cmd->args[1]) & 0xFF;
	}
	else
		exit_status = g_signal & 0xFF;
	write(1, "exit\n", 5);
	return (255 + (exit_status << 8));
}
