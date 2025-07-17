/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:37:18 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/17 17:07:11 by raisufaj         ###   ########.fr       */
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
# include <signal.h>
# include <stdbool.h>
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

// MACROS
# define PROMPT "minishell> "
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

extern int	g_signal;

// EXECUTOR
int			execute(t_node *ast, t_env **env);
int			exec_or(t_node *ast, t_env **env);
int			exec_and(t_node *ast, t_env **env);
int			exec_cmd(t_node *ast, t_env **env);
int			exec_pipe(t_node *ast, t_env **env);
int			exec_group(t_node *ast, t_env **env);

// BUILTINS
int			pwd_exec(t_env *env);
int			exit_exec(t_cmd *cmd);
int			cmp_env(t_env *a, t_env *b);
int			cd_exec(t_cmd *cmd, t_env *env);
int			add_exported(char *arg, t_env **env);
int			export_exec(t_cmd *cmd, t_env **env);
int			export_exec(t_cmd *cmd, t_env **env);
bool		is_valid_arg(char *arg);
bool		change_to_directory(char *path);
bool		validate_path_exists(char *path);
void		get_exported_env(t_env *env);
void		print_exported_value(t_env **sorted_exports);
void		sorted_algo(t_env **sorted_exports, t_env *env, size_t count);
void		set_env_value(t_env **env, const char *key, const char *value);
void		count_exports(t_env *env, t_env **sorted_exports, size_t count);
t_env		*find_env(t_env *env, char *key);
size_t		count_values(t_env *env);

// ENV
t_env		*env_node(void);
t_env		*init_env(char **envp);
t_env		*create_env_from_string(char *env_string);
void		free_env(t_env *env);
void		add_env_to_list(t_env **env, t_env **tail, t_env *new_env);

// MAIN
int			check_args(int argc, char **argv);
int			process_command_line(t_node **ast, t_env *env);
void		cleanup_shell(t_node *ast, t_env *env);
void		init_shell(t_env **env, t_node **ast, char **envp);

// EXPAND
int			check_quotes(char *value);
int			should_word_split(char *new_value, char *old_value);
char		*handle_escaped_dollar(int *i);
char		*handle_literal(char *value, int *i);
char		*get_env_value(t_env *env, char *name);
char		*expand_token(char *value, t_env *env, int last_status);
char		*handle_quotes(char *value, int *i, t_env *env, int last_status);
char		*handle_variable_expression(char *value, int *i, t_env *env,
				int last_status);
char		*consume_unquoted(char *value, int *i);
char		*consume_single_quote(char *value, int *i);
char		*consume_double_quote(char *value, int *i, t_env *env,
				int last_status);
void		expand_wildcards(t_node *node);
void		append_string(char **input, char *str);
void		expand_ast(t_node *node, t_env *env, int last_status);
void		free_token_content(t_token *token, char *new_val, char *old_val);
void		update_token_links(t_token **head, t_token *prev, t_token *current,
				t_token *next);
void		handle_valid_token(t_token **prev, t_token **current,
				char *new_value, char *old_value);
void		word_splitting(t_token **list, t_token **prev, t_token **current,
				char *new_value);

char		*expand_variable(char *value, int *i, t_env *env,
				t_expand_state *state);
t_token		*create_word_tokens(char *expanded_value);

// WILDCARDS
int			match_patt(char *pattern, char *name);
int			should_include_entry(char *pattern, struct dirent *entry);
int			process_directory_entries(char *str, t_token **head, t_token **tail,
				DIR *dir);
void		sort_token(t_token *token);
void		wildcards_cmd(t_node *node);
void		expand_wildcards_in_redir(t_token **redir_list);
void		expand_wildcards_in_tokens(t_token **token_list);
void		add_token_to_list(t_token **head, t_token **tail,
				t_token *new_token);
void		append_files_to_head(t_token **head, t_token *files);
int			handle_star_pattern(char **p, char **n, char **star_p,
				char **star_n);
void		process_expanded_token(t_token **head, t_token **tail,
				t_token *expanded, t_token *current);
void		process_non_expanded_token(t_token **head, t_token **tail,
				t_token *current);
void		process_redir_wildcard_token(t_token **head, t_token **tail,
				t_token *current);
bool		should_expand_wildcard(t_token *token);
t_token		*handle_joker(char *str);
t_token		*create_token_copy(char *name);
t_token		*handle_star_slash_pattern(char *pattern);
t_token		*create_token_with_path(char *dir_path, char *name);
t_token		*handle_simple_wildcard(char *pattern, char *dir_path);

// PARSER
void		free_node(t_node *node);
t_node		*parse_cmd(t_token **tokens);
t_node		*parse_pipe(t_token **tokens);
t_node		*parse_group(t_token **tokens);
t_node		*parse_and_or(t_token **tokens);
t_node		*create_node(t_node_type type);
t_node		*parser(const char *input, t_env *env, int last_status);

// LEXER
int			tokenizer(const char *input, int *i, t_token *new_token);
void		free_tokens(t_token *token);
void		handle_escape(const char *input, int *i, char **buffer);
void		handle_unquoted(const char *input, int *i, char **buffer);
void		pipe_token(const char *input, int *i, t_token *new_token);
void		redir_less_token(const char *input, int *i, t_token *new_token);
void		parenthesis_token(const char *input, int *i, t_token *new_token);
void		redir_great_token(const char *input, int *i, t_token *new_token);
void		head_or_tail(t_token **head, t_token **tail, t_token *new_token);
bool		handle_quote(const char *input, int *i, char **buffer);
bool		word_token(const char *input, int *i, t_token *new_token);
t_token		*lexer(const char *input);

// PROMPT
char		*get_prompt(void);

// UTILS
int			is_space(char c);
int			ft_isspace(int c);
int			is_operator(char c);
int			ft_strcmp(const char *s1, const char *s2);
char		*join_and_free(char *s1, char *s2);
char		*ft_strndup(const char *src, size_t n);
void		parser_error(char *msg);
void		append_token(t_token **head, t_token **tail, t_token *src);
bool		is_redirection(t_token_type type);

// SIGNALS
void		setup_signals(void);
void		setup_signals_exec(void);
void		sigint_handler(int signo);
void		setup_signals_waiting(void);
void		sigint_handler_waiting(int signo);

// EXECUTOR
int			env_exec(t_env *env);
int			echo_exec(t_cmd *cmd);
int			is_builtin(t_cmd *cmd);
int			unset_exec(t_cmd *cmd, t_env **env);
int			exec_external(t_cmd *cmd, t_env *env);
int			exec_builtin(t_cmd *cmd, t_env **env);
int			process_heredocs(t_node *ast, t_env *env);
int			count_heredocs(t_token *redir_list);
int			handle_redir(t_env *env, t_node *ast, t_heredoc_args *args);
int			handle_heredoc(t_env *env, t_node *ast, t_heredoc_args *args);
int			handle_mult_heredoc(t_env *env, t_node *ast, t_heredoc_args *args);
char		*find_path(char *cmd, t_env *env);
char		**env_to_envp(t_env *env);
void		ft_free_split(char **ptr);
void		heredoc_child(int pipe_fd[2], t_env *env, t_node *ast,
				t_heredoc_args *args);
void		sig_heredoc(int status, int *sig);
void		close_files(t_heredoc_args *args);
void		arg_helpers(t_heredoc_args *args, t_token *redir);
t_cmd		*init_cmd(t_node *ast);
t_token		*find_last_heredoc(t_token *redir_list);
bool		heredoc_redir(t_token *current, t_env *env, t_node *ast,
				t_heredoc_args *args);

#endif