/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 00:57:32 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 01:05:15 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

t_env	*env_node(void)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = NULL;
	new_node->value = NULL;
	new_node->exported = true;
	new_node->next = NULL;
	return (new_node);
}

t_env	*create_env_from_string(char *env_string)
{
	t_env	*new_env;
	char	*eq_sign;

	eq_sign = ft_strchr(env_string, '=');
	if (!eq_sign)
		return (NULL);
	new_env = env_node();
	if (!new_env)
		return (NULL);
	new_env->key = ft_substr(env_string, 0, eq_sign - env_string);
	if (!new_env->key)
		return (free(new_env), NULL);
	new_env->value = ft_strdup(eq_sign + 1);
	if (!new_env->value)
		return (free(new_env->key), free(new_env), NULL);
	new_env->next = NULL;
	return (new_env);
}

void	add_env_to_list(t_env **env, t_env **tail, t_env *new_env)
{
	if (!*env)
	{
		*env = new_env;
		*tail = new_env;
	}
	else
	{
		(*tail)->next = new_env;
		*tail = new_env;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*new_env;
	t_env	*env;
	t_env	*tail;
	int		i;

	i = 0;
	env = NULL;
	tail = NULL;
	while (envp[i])
	{
		if (!ft_strchr(envp[i], '='))
		{
			ft_putstr_fd("minishell: invalid environment variable: ", 2);
			ft_putendl_fd(envp[i], 2);
			i++;
			continue ;
		}
		new_env = create_env_from_string(envp[i]);
		if (!new_env)
			return (free_env(env), ft_putendl_fd(
					"minishell: memory allocation failed", 2), NULL);
		add_env_to_list(&env, &tail, new_env);
		i++;
	}
	return (env);
}

void	init_shell(t_env **env, t_node **ast, char **envp)
{
	*env = NULL;
	*ast = NULL;
	g_signal = 0;
	setup_signals();
	*env = init_env(envp);
}
