/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:14:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/04 21:11:25 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	g_signal;

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

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

t_env	*init_env(char **envp)
{
	t_env	*new_env;
	t_env	*env;
	char	*eq_sign;
	int		i;
	size_t	len;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		eq_sign = ft_strchr(envp[i], '=');
		if (!eq_sign)
		{
			ft_putstr_fd(
				"minishell: invalid environment variable: ", 2);
			ft_putendl_fd(envp[i], 2);
			i++;
			continue ;
		}
		new_env = env_node();
		if (!new_env)
			return (free_env(env), ft_putendl_fd(
					"minishell: memory allocation failed", 2), NULL);
		len = eq_sign - envp[i];
		new_env->key = ft_substr(envp[i], 0, len);
		if (!new_env->key)
			return (free(new_env), free_env(env), ft_putendl_fd(
					"minishell: memory allocation failed", 2), NULL);
		new_env->value = ft_strdup(eq_sign + 1);
		if (!new_env->value)
			return (free(new_env->key), free(new_env), free_env(env),
				ft_putendl_fd("minishell: memory allocation failed", 2), NULL);
		new_env->next = env;
		env = new_env;
		i++;
	}
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_node	*ast;
	t_env	*env;

	if (argc != 1)
	{
		write(2, "Usage: ", 7);
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, "\n", 1);
		return (1);
	}
	env = NULL;
	g_signal = 127;
	setup_signals();
	env = init_env(envp);
	if (!env)
		return (1);
	while (1)
	{
		free_node(ast);
		line = get_prompt();
		if (!line)
			break ;
		ast = parsing_file(line, env, g_signal);
		free(line);
		if (!ast)
			continue ;
		g_signal = execute(ast, &env);
	}
	free_env(env);
	rl_clear_history();
	return (g_signal);
}
