/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:37:18 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/06 22:43:39 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "minishell_structs.h"
# include <curses.h>
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>
# include <signal.h>
# include <stdbool.h>

// MACROS
# define PROMPT "minishell> "
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

extern int	g_signal;

// EXECUTOR
int		execute(t_node *ast, t_env **env);
int		exec_cmd(t_node *ast, t_env **env);
int		exec_pipe(t_node *ast, t_env **env);
int		exec_and(t_node *ast, t_env **env);
int		exec_or(t_node *ast, t_env **env);
int		exec_group(t_node *ast, t_env **env);

// BUILTINS
int		pwd_exec(void);
int		cd_exec(t_cmd *cmd);
int		exit_exec(t_cmd *cmd, t_env **env);
int		export_exec(t_cmd *cmd, t_env **env);

// ENV
t_env	*env_node(void);

// EXPAND
char	*expand_token(char *value, t_env *env, int last_status);
char	*get_env_value(t_env *env, char *name);
void	expand_ast(t_node *node, t_env *env, int last_status);
void	expand_wildcards(t_node *node);

// PARSER
t_node	*parse_cmd(t_token **tokens);
t_node	*parsing_file(const char *input, t_env *env, int last_status);

// LEXER
t_token	*lexer(const char *input);
void	parenthesis_token(const char *input, int *i, t_token *new_token);
void	redir_great_token(const char *input, int *i, t_token *new_token);
void	redir_less_token(const char *input, int *i, t_token *new_token);
void	pipe_token(const char *input, int *i, t_token *new_token);
void	free_tokens(t_token *token);
void	free_node(t_node *node);
void	head_or_tail(t_token **head, t_token **tail, t_token *new_token);
int     tokenizer(const char *input, int *i, t_token *new_token);
char	*concat_quote(const char *input, int *i, char *quoted);
void	handle_quote(const char *input, int *i, char **buffer);
void	handle_unquoted(const char *input, int *i, char **buffer);
void	word_token(const char *input, int *i, t_token *new_token);

// PROMPT
char	*get_prompt(void);

// UTILS
void	parser_error(char *msg);
int		is_space(char c);
int		is_operator(char c);
char	*join_and_free(char *s1, char *s2);
char	*ft_strndup(const char *src, size_t n);
int		ft_strcmp(const char *s1, const char *s2);

// SIGNALS
void	sigint_handler(int signo);
void	setup_signals(void);

// PRINT TOKENS
//void	print_tokens(t_token *tokens);

#endif