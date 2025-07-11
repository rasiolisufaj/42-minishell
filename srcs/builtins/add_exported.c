/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_exported.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 14:30:58 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/11 14:47:40 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	extract_key_value(char *arg, char **key, char **value)
{
	char	*eq_sign;
	size_t	len;

	eq_sign = ft_strchr(arg, '=');
	if (!eq_sign)
		len = ft_strlen(arg);
	else
		len = eq_sign - arg;
	*key = ft_substr(arg, 0, len);
	if (!*key)
		return (ft_putendl_fd("minishell: memory allocation failed", 2),
			EXIT_FAILURE);
	if (eq_sign)
	{
		*value = ft_strdup(eq_sign + 1);
		if (!*value)
			return (free(*key),
				ft_putendl_fd("minishell: memory allocation failed", 2),
				EXIT_FAILURE);
	}
	else
		*value = NULL;
	return (EXIT_SUCCESS);
}

static int	update_existing_env(t_env *exist, char *key, char *value)
{
	free(exist->value);
	exist->value = value;
	exist->exported = true;
	free(key);
	return (EXIT_SUCCESS);
}

static int	create_new_env(char *key, char *value, t_env **env)
{
	t_env	*new_env;

	new_env = env_node();
	if (!new_env)
		return (free(key), free(value),
			ft_putendl_fd("minishell: memory allocation failed", 2),
			EXIT_FAILURE);
	new_env->key = key;
	new_env->value = value;
	new_env->exported = true;
	new_env->next = *env;
	*env = new_env;
	return (EXIT_SUCCESS);
}

int	add_exported(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*exist;

	if (extract_key_value(arg, &key, &value) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	exist = find_env(*env, key);
	if (exist)
		return (update_existing_env(exist, key, value));
	return (create_new_env(key, value, env));
}

// Adds or updates an exported variable
/*int	add_exported(char *arg, t_env **env)
{
	t_env	*new_env;
	t_env	*exist;
	char	*eq_sign;
	char	*key;
	char	*new_val;
	size_t	len;

	eq_sign = ft_strchr(arg, '=');
	if (!eq_sign)
		len = ft_strlen(arg);
	else
		len = eq_sign - arg;
	key = ft_substr(arg, 0, len);
	if (!key)
		return (ft_putendl_fd(
				"minishell: memory allocation failed", 2), EXIT_FAILURE);
	exist = find_env(*env, key);
	if (exist)
	{
		if (eq_sign)
		{
			new_val = ft_strdup(eq_sign + 1);
			if (!new_val)
				return (free(key),
					ft_putendl_fd("minishell: memory allocation failed", 2),
					EXIT_FAILURE);
		}
		else
			new_val = NULL;
		free(exist->value);
		exist->value = new_val;
		exist->exported = true;
		free(key);
		return (EXIT_SUCCESS);
	}
	new_env = env_node();
	if (!new_env)
		return (free(key), ft_putendl_fd(
				"minishell: memory allocation failed", 2), EXIT_FAILURE);
	new_env->key = key;
	if (eq_sign)
	{
		new_env->value = ft_strdup(eq_sign + 1);
		if (!new_env->value)
			return (free(new_env->key), free(new_env), free(key),
				ft_putendl_fd("minishell: memory allocation failed", 2),
				EXIT_FAILURE);
	}
	new_env->next = *env;
	*env = new_env;
	return (EXIT_SUCCESS);
}
*/
