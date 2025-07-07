/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:24:55 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/07 19:34:42 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	exec_pipe(t_node *ast, t_env **env)
{
	return (exec_pipe(ast->left, env) || exec_pipe(ast->right, env));
}

int	exec_and(t_node *ast, t_env **env)
{
	int	status;

	status = execute(ast->left, env);
	if (status == 0)
		return (execute(ast->right, env));
	return (status);
}

int	exec_or(t_node *ast, t_env **env)
{
	int	status;

	status = execute(ast->left, env);
	if (status != 0)
		return (execute(ast->right, env));
	return (status);
}

int	exec_group(t_node *ast, t_env **env)
{
	int		status;
	int		wstatus;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		status = execute(ast->left, env);
		exit(status);
	}
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	return (1);
}

int	execute(t_node *ast, t_env **env)
{
	if (!ast)
		return (0);
	if (ast->type == N_CMD)
		return (exec_cmd(ast, env));
	if (ast->type == N_PIPE)
		return (exec_pipe(ast, env));
	if (ast->type == N_AND)
		return (exec_and(ast, env));
	if (ast->type == N_OR)
		return (exec_or(ast, env));
	if (ast->type == N_GROUP)
		return (exec_group(ast, env));
	return (127);
}

/*int main(int argc, char **argv, char **envp)
{
    char   *line;
    t_node *ast;
    t_env  *env;
    int     status;

    env = init_env(envp);
    while (1)
    {
        line = readline("test-shell> ");
        if (!line) break;
        if (*line)
        {
            ast = parsing_file(line, env, 0);
            if (ast)
            {
                status = execute(ast, &env);
                printf("→ exit status = %d\n", status);
                free_node(ast);
            }
        }
        free(line);
    }
    free_env(env);
    return 0;
}*/
