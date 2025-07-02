/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:42:19 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/02 16:38:42 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	unset_exec(t_cmd *cmd, t_env **env)
{
	int		last_status;
	int		i;
	t_env	*current;
	t_env	*prev;

	if (env == NULL || cmd == NULL || cmd->args == NULL)
		return (EXIT_FAILURE);
	last_status = EXIT_SUCCESS;
	i = 1;
	while (cmd->args[i])
	{
		if (is_valid_arg(cmd->args[i]) == false)
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd(": not a valid identifier", 2);
			last_status = EXIT_FAILURE;
		}
		else
		{
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
					break ;
				}
				prev = current;
				current = current->next;
			}
		}
		i++;
	}
	return (last_status);
}
