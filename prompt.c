/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 08:33:35 by itaharbo          #+#    #+#             */
/*   Updated: 2025/05/22 12:03:55 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "readline/readline.h"
#include "readline/history.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int	g_signal;

/*
printf("%s: command not found\n", input);
printf("%s: %s: No such file or directory\n", cmd, arg);
printf("%s: %s: Permission denied\n", cmd, arg);
printf("%s: %s: Is a directory\n", cmd, arg);
printf("%s: cannot access '%s': No such file or directory\n", cmd, arg);
*/

int	ft_is_space(char *str)
{
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(void)
{
	char		*input;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			if (ft_is_space(input))
			{
				free(input);
				continue ;
			}
			printf("%s: command not found\n", input);
		}
		free(input);
//		wait(NULL);
	}
	rl_clear_history();
	return (0);
}
