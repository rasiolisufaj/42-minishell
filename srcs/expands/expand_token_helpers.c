/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:42:23 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/10 22:13:15 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

char	*handle_quotes(char *value, int *i, t_env *env,
		int last_status)
{
	char	*part;

	if (value[*i] == '\'')
		part = consume_single_quote(value, i);
	else
		part = consume_double_quote(value, i, env, last_status);
	return (part);
}

char	*handle_escaped_dollar(int *i)
{
	char	*ret;

	*i += 2;
	ret = ft_strdup("$");
	if (!ret)
		return (ft_putendl_fd(
				"Error: Memory allocation failed", 2), NULL);
	return (ret);
}

char	*handle_variable_expression(char *value, int *i, t_env *env,
		int last_status)
{
	char			*part;
	t_expand_state	state;

	state.last_status = last_status;
	state.in_double_quotes = 0;
	part = expand_variable(value, i, env, &state);
	return (part);
}

char	*handle_literal(char *value, int *i)
{
	char	*part;

	part = consume_unquoted(value, i);
	return (part);
}
