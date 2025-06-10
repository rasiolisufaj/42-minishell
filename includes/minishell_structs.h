/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_structs.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:35:46 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/10 20:41:23 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCTS_H
# define MINISHELL_STRUCTS_H

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
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

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
}	t_node;

#endif
