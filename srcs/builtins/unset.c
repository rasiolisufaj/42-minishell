/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:42:19 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/11 23:43:39 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	isnot_valid_msg(char *arg)
{
	ft_putstr_fd("unset: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": not a valid identifier", 2);
}

int	unset_cpy(t_cmd *cmd, t_env **env, int i)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, cmd->args[i]) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	unset_exec(t_cmd *cmd, t_env **env)
{
	int		last_status;
	int		i;

	if (env == NULL || cmd == NULL || cmd->args == NULL)
		return (EXIT_FAILURE);
	last_status = EXIT_SUCCESS;
	i = 1;
	while (cmd->args[i])
	{
		if (is_valid_arg(cmd->args[i]) == false)
		{
			isnot_valid_msg(cmd->args[i]);
			last_status = EXIT_FAILURE;
		}
		else
		{
			unset_cpy(cmd, env, i);
		}
		i++;
	}
	return (last_status);
}
