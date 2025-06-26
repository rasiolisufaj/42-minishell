/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:37:18 by raisufaj          #+#    #+#             */
/*   Updated: 2025/06/26 22:40:58 by itaharbo         ###   ########.fr       */
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

// MACROS
# define PROMPT "minishell> "

//EXPAND
char	*expand_token(char *value, t_env *env, int last_status);
char	*get_env_value(t_env *env, char *name);
void	expand_ast(t_node *node, t_env *env, int last_status);
void	expand_wildcards(t_node *node);

//PARSER
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
void	tokenizer(const char *input, int *i, t_token *new_token);
char	*concat_quote(const char *input, int *i, char *quoted);
void	handle_quote(const char *input, int *i, char **buffer);
void	handle_unquoted(const char *input, int *i, char **buffer);
void	word_token(const char *input, int *i, t_token *new_token);

// PROMPT
char	*get_prompt(void);

// UTILS
void	parser_error(const char *msg);
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