/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_unquoted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:45:32 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/10 21:45:58 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

char	*consume_unquoted(char *value, int *i)
{
	char	*part;
	int		start;

	start = *i;
	while (value[*i] && value[*i] != '\'' && value[*i] != '"'
		&& value[*i] != '$' && !(value[*i] == '\\' && value[*i + 1] == '$'))
		(*i)++;
	part = ft_strndup(value + start, *i - start);
	if (!part)
		return (ft_putendl_fd(
				"Error: Memory allocation failed in consume_unquoted",
				2), NULL);
	return (part);
}
