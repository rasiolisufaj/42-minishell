/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_token_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 00:50:00 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 01:11:23 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

t_token	*create_token_with_path(char *dir_path, char *name)
{
	t_token	*token;
	char	*tmp;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (ft_strcmp(dir_path, ".") == 0)
		token->value = strdup(name);
	else
	{
		tmp = ft_strjoin(dir_path, "/");
		token->value = ft_strjoin(tmp, name);
		free(tmp);
	}
	if (!token->value)
		return (free(token), NULL);
	token->type = T_WORD;
	token->next = NULL;
	return (token);
}

t_token	*create_token_copy(char *name)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = strdup(name);
	if (!token->value)
		return (free(token), NULL);
	token->type = T_WORD;
	token->next = NULL;
	return (token);
}

void	add_token_to_list(t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		*tail = new_token;
	}
}

void	sort_token(t_token *token)
{
	t_token	*current;
	t_token	*next;
	char	*tmp;

	if (!token)
		return ;
	current = token;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcmp(current->value, next->value) > 0)
			{
				tmp = current->value;
				current->value = next->value;
				next->value = tmp;
			}
			next = next->next;
		}
		current = current->next;
	}
}

void	append_files_to_head(t_token **head, t_token *files)
{
	t_token	*tail;

	if (!*head)
		*head = files;
	else
	{
		tail = *head;
		while (tail->next)
			tail = tail->next;
		tail->next = files;
	}
}
