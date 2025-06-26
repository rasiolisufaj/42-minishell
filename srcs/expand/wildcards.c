/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:38:42 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/26 22:26:25 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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

int	match_patt(char *pattern, char *name)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	start = -1;
	while (name[j])
	{
		if (pattern[i] == '*')
			start = i++;
		else if (pattern[i] == name[j])
		{
			i++;
			j++;
		}
		else if (start >= 0)
		{
			i = start + 1;
			j++;
		}
		else
			return (0);
	}
	while (pattern[i] == '*')
		i++;
	return (pattern[i] == '\0');
}

t_token	*handle_joker(char *str)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*head;
	t_token			*tail;
	t_token			*tokens;

	if (!str || !*str)
		return (NULL);
	dir = opendir(".");
	if (!dir)
	{
		perror("opendir");
		return (NULL);
	}
	head = NULL;
	tail = NULL;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strcmp(entry->d_name, ".") != 0
			&& ft_strcmp(entry->d_name, "..") != 0
			&& match_patt(str, entry->d_name)
			&& (str[0] == '.' || entry->d_name[0] != '.'))
		{
			tokens = malloc(sizeof(t_token));
			if (!tokens)
			{
				perror("malloc");
				closedir(dir);
				return (NULL);
			}
			tokens->value = strdup(entry->d_name);
			if (!tokens->value)
			{
				free(tokens);
				perror("strdup");
				closedir(dir);
				return (NULL);
			}
			tokens->type = T_WORD;
			tokens->next = NULL;
			if (!head)
			{
				head = tokens;
				tail = tokens;
			}
			else
			{
				tail->next = tokens;
				tail = tokens;
			}
		}
		entry = readdir(dir);
	}
	sort_token(head);
	closedir(dir);
	return (head);
}

void	wildcards_cmd(t_node *node)
{
	t_token	*head;
	t_token	*tail;
	t_token	*current;
	t_token	*expanded;
	t_token	*next;

	head = NULL;
	tail = NULL;
	current = node->tokens;
	while (current)
	{
		next = current->next;
		if (ft_strchr(current->value, '*'))
		{
			expanded = handle_joker(current->value);
			if (expanded)
			{
				if (!head)
				{
					head = expanded;
					tail = expanded;
				}
				else
					tail->next = expanded;
				while (tail->next)
					tail = tail->next;
				free(current->value);
				free(current);
			}
			else
			{
				current->next = NULL;
				if (!head)
				{
					head = current;
					tail = current;
				}
				else
				{
					tail->next = current;
					tail = current;
				}
			}
		}
		else
		{
			current->next = NULL;
			if (!head)
			{
				head = current;
				tail = current;
			}
			else
			{
				tail->next = current;
				tail = current;
			}
		}
		current = next;
	}
	node->tokens = head;
}

void	expand_wildcards(t_node *node)
{
	if (!node)
		return ;
	if (node->type != N_CMD)
	{
		expand_wildcards(node->left);
		expand_wildcards(node->right);
	}
	else
		wildcards_cmd(node);
}
