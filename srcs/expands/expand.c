/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:28:38 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/10 23:12:27 by itaharbo         ###   ########.fr       */
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
	t_token	*current;
	t_token	*next;
	t_token	*prev;
	char	*old_value;
	char	*new_value;

	current = *token_list;
	while (current)
	{
		next = current->next;
		old_value = current->value;
		new_value = expand_token(old_value, env, last_status);
		if (!new_value)
			return (ft_putendl_fd("Error: Memory allocation failed", 2));
		if (!*new_value && check_quotes(old_value) == 0)
		{
			free_token_content(current, new_value, old_value);
			update_token_links(token_list, prev, current, next);
			continue ;
		}
		current->value = new_value;
		free(old_value);
		prev = current;
		current = next;
	}
}

void	expand_redir_list(t_token **redir_list, t_env *env, int last_status)
{
	t_token	*current;
	t_token	*next;
	t_token	*prev;
	char	*old_value;
	char	*new_value;

	current = *redir_list;
	while (current)
	{
		next = current->next;
		old_value = current->value;
		new_value = expand_token(old_value, env, last_status);
		if (!new_value)
			return (ft_putendl_fd("Error: Memory allocation failed", 2));
		if (!*new_value)
		{
			free_token_content(current, new_value, old_value);
			update_token_links(redir_list, prev, current, next);
			continue ;
		}
		current->value = new_value;
		free(old_value);
		prev = current;
		current = next;
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
