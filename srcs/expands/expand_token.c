/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:13:15 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/10 20:56:35 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

char	*expand_token(char *value, t_env *env, int last_status)
{
	char	*result;
	char	*part;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			part = handle_quotes(value, &i, env, last_status);
		else if (value[i] == '\\' && value[i + 1] == '$')
			part = handle_escaped_dollar(&i);
		else if (value[i] == '$')
			part = handle_variable_expression(value, &i, env, last_status);
		else
			part = handle_literal(value, &i);
		if (!part)
			return (free(result), NULL);
		append_string(&result, part);
		free(part);
	}
	return (result);
}
