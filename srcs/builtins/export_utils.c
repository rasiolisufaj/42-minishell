/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:11:18 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/17 15:18:59 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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
		if (current->exported && current->key)
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
