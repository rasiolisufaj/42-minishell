/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:30:55 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/17 16:25:13 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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
