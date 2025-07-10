/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume_single_quote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:44:38 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/10 21:45:01 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

char	*consume_single_quote(char *value, int *i)
{
	char	*part;
	int		start;

	(*i)++;
	start = *i;
	while (value[*i] && value[*i] != '\'')
		(*i)++;
	part = ft_strndup(value + start, *i - start);
	if (!part)
		return (ft_putendl_fd(
				"Error: Memory allocation failed in consume_single_quote",
				2), NULL);
	if (value[*i] == '\'')
		(*i)++;
	return (part);
}
