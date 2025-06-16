/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:14:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/16 12:05:40 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*tokens;

	(void)argc;
	(void)argv;
	(void)envp;
	setup_signals();
	while (1)
	{
		line = get_prompt();
		if (!line)
			continue ;
		tokens = lexer(line);
		if (tokens)
			print_tokens(tokens); // This is temporary for checking LEXER output
		else
		{
			printf("Error: Failed to tokenize input.\n");
			free(line);
			continue ;
		}
		// TODO: parse_tokens(tokens)
		// TODO: execute_commands(parsed_cmds)
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
