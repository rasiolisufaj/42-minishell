/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:30:55 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/07 17:50:48 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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

int	add_exported(char *arg, t_env **env)
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
			ft_putstr_fd("export: ", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd(": not a valid identifier", 2);
			status = 1;
		}
		else if (add_exported(cmd->args[i], env) != EXIT_SUCCESS)
			status = 1;
		i++;
	}
	return (status);
}

// extern size_t   count_values(t_env *env);
// extern void     sorted_algo(t_env **sorted_exports,
//						t_env *env, size_t count);
// extern void     print_exported_value(t_env **sorted_exports);
// extern t_env   *env_node(void);

// static void add_node(t_env **env, const char *k, const char *v, int exp)
// {
//     t_env *n = env_node();
//     if (!n) { perror("env_node"); exit(1); }

//     n->key      = strdup(k);
//     n->value    = v ? strdup(v) : NULL;
//     n->exported = exp;
//     n->next     = *env;
//     *env        = n;
// }

// int main(void)
// {
//     t_env *env = NULL;
//     size_t cnt;

//     /* 1) Construire la liste : certaines variables exportées */
//     add_node(&env, "oko",         "(     lol pop p)",    1);
//     add_node(&env, "aa",         "      a-val",    1);
//     add_node(&env, "AA",         "A-val",    1);
//     add_node(&env, "BB",         NULL,       1);
//     add_node(&env, "NOT_EXPORTED","no",      0);

//     /* 2) Tester count_values */
//     cnt = count_values(env);
//     printf("count_values = %zu (attendu 4)\n", cnt);

//     /* 3) Trier et afficher les exports */
//     t_env **arr = malloc((cnt + 1) * sizeof *arr);
//     if (!arr) { perror("malloc"); return 1; }
//     sorted_algo(arr, env, cnt);

//     printf("=== sorted exports ===\n");
//     print_exported_value(arr);
//     free(arr);

//     /* 4) Nettoyage (libération de la liste env) */
//     while (env)
//     {
//         t_env *next = env->next;
//         free(env->key);
//         free(env->value);
//         free(env);
//         env = next;
//     }

//     return 0;
// }
