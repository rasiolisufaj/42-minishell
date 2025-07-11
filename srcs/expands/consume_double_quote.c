/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_double_quote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:09:02 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/10 22:14:42 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static char	*dq_escaped_dollar(int *i)
{
	char	*part;

	part = ft_strdup("$");
	if (!part)
		return (ft_putendl_fd(
				"Error: Memory allocation failed", 2), NULL);
	*i += 2;
	return (part);
}

static char	*dq_literal_and_var(char *value, int *i, t_env *env,
		int last_status)
{
	int				start;
	char			*ret;
	t_expand_state	state;

	state.last_status = last_status;
	state.in_double_quotes = 1;
	if (value[*i] == '$')
		return (expand_variable(value, i, env, &state));
	start = *i;
	while (value[*i] && value[*i] != '"' && value[*i] != '$'
		&& !(value[*i] == '\\' && value[*i + 1] == '$'))
		(*i)++;
	if (start >= *i)
		return (NULL);
	ret = ft_strndup(value + start, *i - start);
	if (!ret)
		return (ft_putendl_fd(
				"Error: Memory allocation failed", 2), NULL);
	return (ret);
}

char	*consume_double_quote(char *value, int *i,
	t_env *env, int last_status)
{
	char	*part;
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (ft_putendl_fd(
				"Error: Memory allocation failed", 2), NULL);
	(*i)++;
	while (value[*i] && value[*i] != '"')
	{
		if (value[*i] == '\\' && value[*i + 1] == '$')
			part = dq_escaped_dollar(i);
		else
			part = dq_literal_and_var(value, i, env, last_status);
		if (!part)
			return (free(result), NULL);
		append_string(&result, part);
		free(part);
		if (!result)
			return (NULL);
	}
	if (value[*i] == '"')
		(*i)++;
	return (result);
}
