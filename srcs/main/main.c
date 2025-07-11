/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:14:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/12 01:05:40 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	g_signal;

static int	get_main_exit_status(void)
{
	if (g_signal >= 255)
		return ((g_signal >> 8) & 0xFF);
	return (g_signal & 0xFF);
}

int	check_args(int argc, char **argv)
{
	if (argc != 1)
		return (write(2, "Usage: ", 7), write(2, argv[0],
				ft_strlen(argv[0])), write(2, "\n", 1), 1);
	return (0);
}

int	process_command_line(t_node **ast, t_env *env)
{
	char	*line;

	free_node(*ast);
	*ast = NULL;
	line = get_prompt();
	if (!line)
		return (0);
	*ast = parser(line, env, g_signal);
	free(line);
	if (!*ast)
	{
		g_signal = 2;
		return (1);
	}
	return (2);
}

void	cleanup_shell(t_node *ast, t_env *env)
{
	free_node(ast);
	free_env(env);
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_node	*ast;
	t_env	*env;
	int		status;

	if (check_args(argc, argv))
		return (1);
	init_shell(&env, &ast, envp);
	if (!env)
		return (1);
	while (1)
	{
		status = process_command_line(&ast, env);
		if (status == 0)
			break ;
		if (status == 1)
			continue ;
		g_signal = execute(ast, &env);
		if (g_signal >= 255)
			break ;
	}
	cleanup_shell(ast, env);
	return (get_main_exit_status());
}
