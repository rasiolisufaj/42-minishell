/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:25:12 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/30 22:50:49 by itaharbo         ###   ########.fr       */
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

int	pwd_exec(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("getcwd");
		return (EXIT_FAILURE);
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (EXIT_SUCCESS);
}

int	export(t_cmd *cmd, t_env *env)
{
	int	i;

	if (env == NULL)
		return (EXIT_FAILURE);
	if (cmd->ac == 1)
		return (env_cmd(env));
	else
	{
		
	}
	return (EXIT_SUCCESS);
}

int	env_cmd(t_env *env)
{
	t_env	*current;

	if (env == NULL)
		return (EXIT_SUCCESS);
	current = env;
	while (current)
	{
		if (current->key && current->value)
		{
			write(1, current->key, ft_strlen(current->key));
			write(1, "=", 1);
			write(1, current->value, ft_strlen(current->value));
			write(1, "\n", 1);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	is_numeric_argument(char *arg)
{
	int i;

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
	int last_status;

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
