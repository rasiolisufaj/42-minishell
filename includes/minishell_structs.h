/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_structs.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:35:46 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/15 20:47:10 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCTS_H
# define MINISHELL_STRUCTS_H
# include <stdbool.h>

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_AND,
	T_OR,
	T_PAREN_OPEN,
	T_PAREN_CLOSE,
	T_UNKNOWN,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_token_lists
{
	t_token		*args_head;
	t_token		*args_tail;
	t_token		*redir_head;
	t_token		*redir_tail;
}	t_token_lists;

typedef enum e_node_type
{
	N_CMD,
	N_PIPE,
	N_AND,
	N_OR,
	N_GROUP,
}	t_node_type;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_token			*tokens;
	t_token			*redir_list;
}	t_node;

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			exported;
	struct s_env	*next;
}	t_env;

typedef struct s_expand_state
{
	int		last_status;
	bool	in_double_quotes;
}	t_expand_state;

typedef struct s_token_ptrs
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;
}	t_token_ptrs;

typedef struct s_cmd
{
	char	**args;
	int		ac;
}	t_cmd;

typedef struct s_heredoc_args
{
	t_cmd	*cmd;
	char	*delimiter;
	int		saved_in;
	int		saved_out;
}	t_heredoc_args;

#endif
