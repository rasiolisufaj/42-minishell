/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 22:52:49 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/11 21:18:54 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_prompt(void)
{
	char	*line;
	int		i;

	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			return (printf("exit\n"), NULL);
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		i = 0;
		while (line[i] && line[i] == ' ')
			i++;
		if (line[i] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		return (line);
	}
}
