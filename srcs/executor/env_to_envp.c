/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:52:19 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/17 15:19:34 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	free_envp_part(char **envp, int start)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (i < start)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	count_env_nodes(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*create_env_entry(t_env *env_node)
{
	char	*tmp;
	char	*entry;

	tmp = ft_strjoin(env_node->key, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, env_node->value);
	free(tmp);
	if (!entry)
		return (NULL);
	return (entry);
}

char	**env_to_envp(t_env *env)
{
	t_env	*current;
	char	**envp;
	int		i;
	int		count;

	count = count_env_nodes(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	current = env;
	i = 0;
	while (current && i < count)
	{
		if (current->exported && current->value)
		{
			envp[i] = create_env_entry(current);
			if (!envp[i])
				return (free_envp_part(envp, i), NULL);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}
