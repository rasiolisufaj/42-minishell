/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:28:38 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/11 21:29:20 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	append_string(char **input, char *str)
{
	char	*tmp;

	if (!str || !*input)
		return ;
	tmp = ft_strjoin(*input, str);
	if (!tmp)
	{
		ft_putendl_fd(
			"Error: Memory allocation failed in append_string", 2);
		free(*input);
		*input = NULL;
		return ;
	}
	free(*input);
	*input = tmp;
}

char	*get_env_value(t_env *env, char *var_name)
{
	t_env	*current;

	if (!env || !var_name || !*var_name)
		return (NULL);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	expand_token_list(t_token **token_list, t_env *env, int last_status)
{
	t_token_ptrs	ptrs;
	char			*old_value;
	char			*new_value;

	ptrs.current = *token_list;
	ptrs.prev = NULL;
	while (ptrs.current)
	{
		ptrs.next = ptrs.current->next;
		old_value = ptrs.current->value;
		new_value = expand_token(old_value, env, last_status);
		if (!new_value)
			return (ft_putendl_fd("Error: Memory allocation failed", 2));
		if (!*new_value && check_quotes(old_value) == 0)
		{
			free_token_content(ptrs.current, new_value, old_value);
			update_token_links(token_list, ptrs.prev, ptrs.current, ptrs.next);
			ptrs.current = ptrs.next;
			continue ;
		}
		if (should_word_split(new_value, old_value))
			word_splitting(token_list, &ptrs.prev, &ptrs.current, new_value);
		else
			handle_valid_token(&ptrs.prev, &ptrs.current, new_value, old_value);
	}
}

void	expand_redir_list(t_token **redir_list, t_env *env, int last_status)
{
	t_token_ptrs	ptrs;
	char			*old_value;
	char			*new_value;

	ptrs.current = *redir_list;
	ptrs.prev = NULL;
	while (ptrs.current)
	{
		ptrs.next = ptrs.current->next;
		old_value = ptrs.current->value;
		new_value = expand_token(old_value, env, last_status);
		if (!new_value)
			return (ft_putendl_fd("Error: Memory allocation failed", 2));
		if (!*new_value)
		{
			free_token_content(ptrs.current, new_value, old_value);
			update_token_links(redir_list, ptrs.prev, ptrs.current, ptrs.next);
			ptrs.current = ptrs.next;
			continue ;
		}
		handle_valid_token(&ptrs.prev, &ptrs.current, new_value, old_value);
	}
}

void	expand_ast(t_node *node, t_env *env, int last_status)
{
	if (!node)
		return ;
	if (node->type != N_CMD)
	{
		expand_ast(node->left, env, last_status);
		expand_ast(node->right, env, last_status);
		return ;
	}
	expand_token_list(&node->tokens, env, last_status);
	expand_redir_list(&node->redir_list, env, last_status);
}
