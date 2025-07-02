/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:44:41 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/02 17:47:51 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	env_exec(t_env *env)
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
