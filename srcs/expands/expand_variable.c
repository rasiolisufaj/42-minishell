/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:12:27 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/11 20:16:54 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static char	*ev_handle_dollar(char *value, int *i, t_expand_state *state)
{
	char	*part;

	if (value[*i] == '?')
	{
		part = ft_itoa(state->last_status);
		if (part)
			(*i)++;
	}
	else
	{
		(*i)++;
		part = ft_strdup("");
		if (!part)
			return (NULL);
	}
	return (part);
}

static char	*ev_handle_dq(int *i, t_expand_state *state)
{
	char	*part;

	(*i)++;
	if (state->in_double_quotes)
	{
		part = ft_strdup("$");
		if (!part)
			return (NULL);
	}
	else
	{
		part = ft_strdup("");
		if (!part)
			return (NULL);
	}
	return (part);
}

static char	*ev_invalid_char(void)
{
	char	*part;

	part = ft_strdup("$");
	if (!part)
		return (NULL);
	return (part);
}

static char	*ev_valid_char(char *value, t_env *env, int *i)
{
	char	*part;
	char	*key;
	char	*env_value;
	int		start;

	start = *i;
	while (value[*i] && (value[*i] == '_' || ft_isalnum(value[*i])))
		(*i)++;
	key = ft_strndup(value + start, *i - start);
	if (!key)
		return (NULL);
	env_value = get_env_value(env, key);
	free(key);
	if (!env_value || !*env_value)
	{
		part = ft_strdup("");
		if (!part)
			return (NULL);
		return (part);
	}
	part = ft_strdup(env_value);
	if (!part)
		return (NULL);
	return (part);
}

char	*expand_variable(char *value, int *i, t_env *env,
	t_expand_state *state)
{
	char	*part;

	(*i)++;
	if (value[*i] == '?' || value[*i] == '*' || value[*i] == '@'
		|| ft_isdigit(value[*i]))
		part = ev_handle_dollar(value, i, state);
	else if (value[*i] == '"' || value[*i] == '\'')
		part = ev_handle_dq(i, state);
	else if (!ft_isalpha(value[*i]) && value[*i] != '_')
		part = ev_invalid_char();
	else
		part = ev_valid_char(value, env, i);
	if (!part)
		return (ft_putendl_fd(
				"Error: Memory allocation failed", 2), NULL);
	return (part);
}
