/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:30:55 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/11 15:15:08 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"
/*
// Counts how many env nodes are marked as exported
size_t	count_values(t_env *env)
{
	t_env	*current;
	size_t	count;

	if (!env)
		return (0);
	count = 0;
	current = env;
	while (current)
	{
		if (current->exported)
			count++;
		current = current->next;
	}
	return (count);
}

// Fills an array sorted_exports with exported t_env* pointers
void	count_exports(t_env *env, t_env **sorted_exports, size_t count)
{
	t_env	*current;
	size_t	i;

	current = env;
	i = 0;
	while (current && i < count)
	{
		if (current->exported)
			sorted_exports[i++] = current;
		current = current->next;
	}
	sorted_exports[count] = NULL;
}

// Compares two t_env variables
int	cmp_env(t_env *a, t_env *b)
{
	int	cmp_res;

	cmp_res = ft_strcmp(a->key, b->key);
	if (cmp_res != 0)
		return (cmp_res);
	if (!a->value && !b->value)
		return (0);
	else if (!a->value)
		return (-1);
	else if (!b->value)
		return (1);
	return (ft_strcmp(a->value, b->value));
}

// Sorts the sorted_exports array by using bubble sort and cmp_env
void	sorted_algo(t_env **sorted_exports, t_env *env, size_t count)
{
	size_t	i;
	size_t	j;
	t_env	*tmp;

	if (!sorted_exports || !env || count < 2)
		return ;
	count_exports(env, sorted_exports, count);
	i = 0;
	while (i + 1 < count)
	{
		j = 0;
		while (j + 1 < count - i)
		{
			if (cmp_env(sorted_exports[j],
					sorted_exports[j + 1]) > 0)
			{
				tmp = sorted_exports[j];
				sorted_exports[j] = sorted_exports[j + 1];
				sorted_exports[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

// Print the exported variables
void	print_exported_value(t_env **sorted_exports)
{
	t_env	**current;
	char	*value;

	current = sorted_exports;
	while (*current)
	{
		value = (*current)->value;
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd((*current)->key, 1);
		if (value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		current++;
	}
}
*/

// Display all exported variables in sorted order
void	get_exported_env(t_env *env)
{
	size_t	count;
	t_env	**sorted_exports;

	if (!env)
		return ;
	count = count_values(env);
	sorted_exports = malloc(sizeof(t_env *) * (count + 1));
	if (!sorted_exports)
	{
		perror("malloc in get_exported_env failed");
		return ;
	}
	sorted_algo(sorted_exports, env, count);
	print_exported_value(sorted_exports);
	free(sorted_exports);
}

// Checks if an argument is a valid environment variable name
bool	is_valid_arg(char *arg)
{
	size_t	i;

	if (!arg || arg[0] == '\0')
		return (FALSE);
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (FALSE);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

// Searches the linked list for a node with a matching key
t_env	*find_env(t_env *env, char *key)
{
	t_env	*current;

	if (!env || !key || !*key)
		return (NULL);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	export_exec(t_cmd *cmd, t_env **env)
{
	int	i;
	int	status;

	if (env == NULL)
		return (EXIT_FAILURE);
	if (cmd->ac == 1)
		return (get_exported_env(*env), EXIT_SUCCESS);
	status = 0;
	i = 1;
	while (i < cmd->ac)
	{
		if (!is_valid_arg(cmd->args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else if (add_exported(cmd->args[i], env) != EXIT_SUCCESS)
			status = 1;
		i++;
	}
	return (status);
}
