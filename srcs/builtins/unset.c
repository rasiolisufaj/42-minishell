/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:42:19 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 00:23:19 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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
	int		i;

	if (env == NULL || cmd == NULL || cmd->args == NULL)
		return (EXIT_FAILURE);
	i = 1;
	while (cmd->args[i])
	{
		unset_cpy(cmd, env, i);
		i++;
	}
	return (EXIT_SUCCESS);
}
