/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:52:49 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/15 23:09:37 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_prompt(void)
{
	char	*line;

	line = readline(PROMPT);
	if (!line)
	{
		printf("\n");
		exit(EXIT_SUCCESS);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	if (ft_strncmp(line, "exit", 5) == 0)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}
	add_history(line);
	return (line);
}
