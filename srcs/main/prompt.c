/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:52:49 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/30 17:58:41 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_prompt(void)
{
	char	*line;

	line = readline(PROMPT);
	if (!line)
		return (printf("\n"), NULL);
	if (line[0] == '\0')
		return (free(line), NULL);
	add_history(line);
	return (line);
}
