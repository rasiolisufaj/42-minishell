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

int	check_args(int argc, char **argv)
{
	if (argc != 1)
		return (write(2, "Usage: ", 7), write(2, argv[0],
				ft_strlen(argv[0])), write(2, "\n", 1), 1);
	return (0);
}

// Heredocs are read before expansion so the delimiter stays literal.
// Words are expanded later, in exec_cmd, right before each command runs.
static int	prepare_line(t_shell *sh)
{
	int	ret;

	ret = read_heredocs(sh->ast, sh);
	if (ret == 0)
		return (2);
	sh->last_status = ret;
	unlink_heredocs(sh->ast);
	return (1);
}

int	process_command_line(t_shell *sh)
{
	char	*line;

	free_node(sh->ast);
	sh->ast = NULL;
	g_signal = 0;
	line = get_prompt();
	if (g_signal == SIGINT)
		sh->last_status = 130;
	if (!line)
		return (0);
	sh->ast = parser(line);
	free(line);
	if (!sh->ast)
	{
		sh->last_status = 2;
		return (1);
	}
	return (prepare_line(sh));
}

void	cleanup_shell(t_node *ast, t_env *env)
{
	free_node(ast);
	free_env(env);
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	int		ret;

	if (check_args(argc, argv))
		return (1);
	init_shell(&sh, envp);
	while (1)
	{
		ret = process_command_line(&sh);
		if (ret == 0)
			break ;
		if (ret == 1)
			continue ;
		setup_signals_waiting();
		sh.last_status = execute(sh.ast, &sh);
		setup_signals();
		unlink_heredocs(sh.ast);
		if (sh.last_status >= EXIT_FLAG)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
	}
	cleanup_shell(sh.ast, sh.env);
	return (sh.last_status & 0xFF);
}
