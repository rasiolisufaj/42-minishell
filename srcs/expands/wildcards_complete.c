/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_complete.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:38:42 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/10 17:38:02 by itaharbo         ###   ########.fr       */
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
	char	*p;
	char	*n;
	char	*star_p;
	char	*star_n;

	p = pattern;
	n = name;
	star_p = NULL;
	star_n = NULL;
	while (*n)
	{
		if (*p == '*')
		{
			star_p = p++;
			star_n = n;
		}
		else if (*p == *n)
		{
			p++;
			n++;
		}
		else if (star_p)
		{
			p = star_p + 1;
			n = ++star_n;
		}
		else
			return (0);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}

t_token	*create_token(char *name)
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

int	should_include_entry(char *pattern, struct dirent *entry)
{
	return (ft_strcmp(entry->d_name, ".") != 0
		&& ft_strcmp(entry->d_name, "..") != 0
		&& match_patt(pattern, entry->d_name)
		&& (pattern[0] == '.' || entry->d_name[0] != '.'));
}

int	process_directory_entries(char *str, t_token **head,
	t_token **tail, DIR *dir)
{
	struct dirent	*entry;
	t_token			*new_token;

	entry = readdir(dir);
	while (entry != NULL)
	{
		if (should_include_entry(str, entry))
		{
			new_token = create_token(entry->d_name);
			if (!new_token)
				return (perror("malloc"), 0);
			add_token_to_list(head, tail, new_token);
		}
		entry = readdir(dir);
	}
	return (1);
}

t_token	*handle_simple_wildcard(char *pattern, char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*head;
	t_token			*tail;
	t_token			*new_token;

	dir = opendir(dir_path);
	if (!dir)
		return (NULL);
	head = NULL;
	tail = NULL;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (should_include_entry(pattern, entry))
		{
			new_token = create_token_with_path(dir_path, entry->d_name);
			if (!new_token)
				return (closedir(dir), NULL);
			add_token_to_list(&head, &tail, new_token);
		}
		entry = readdir(dir);
	}
	sort_token(head);
	return (closedir(dir), head);
}

void	append_files_to_head(t_token **head, t_token *files)
{
	if (!*head)
		*head = files;
	else
	{
		t_token *tail = *head;
		while (tail->next)
			tail = tail->next;
		tail->next = files;
	}
}

t_token	*handle_star_slash_pattern(char *pattern)
{
	t_token	*dirs;
	t_token	*head;
	t_token	*current;
	t_token *files;
	struct stat statbuf;

	dirs = handle_simple_wildcard("*", ".");
	head = NULL;
	current = dirs;
	while (current)
	{
		if (stat(current->value, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			files = handle_simple_wildcard(pattern, current->value);
			if (files)
				append_files_to_head(&head, files);
		}
		current = current->next;
	}
	sort_token(head);
	return (head);
}

t_token	*handle_joker(char *str)
{
	char	*slash_pos;
	char	*dir_path;
	char	*pattern;
	t_token	*result;

	if (!str || !*str)
		return (NULL);
	if (ft_strncmp(str, "*/", 2) == 0)
		return (handle_star_slash_pattern(str + 2));
	slash_pos = ft_strrchr(str, '/');
	if (slash_pos)
	{
		dir_path = ft_substr(str, 0, slash_pos - str);
		pattern = ft_substr(str, (slash_pos - str) + 1, ft_strlen(str));
		result = handle_simple_wildcard(pattern, dir_path);
		free(dir_path);
		free(pattern);
		return (result);
	}
	else
		return (handle_simple_wildcard(str, "."));
}

void	process_expanded_token(t_token **head, t_token **tail,
		t_token *expanded, t_token *current)
{
	if (!*head)
	{
		*head = expanded;
		*tail = expanded;
	}
	else
		(*tail)->next = expanded;
	while ((*tail)->next)
		*tail = (*tail)->next;
	free(current->value);
	free(current);
}

void	process_non_expanded_token(t_token **head, t_token **tail,
		t_token *current)
{
	current->next = NULL;
	if (!*head)
	{
		*head = current;
		*tail = current;
	}
	else
	{
		(*tail)->next = current;
		*tail = current;
	}
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
				process_expanded_token(&head, &tail, expanded, current);
			else
				process_non_expanded_token(&head, &tail, current);
		}
		else
			process_non_expanded_token(&head, &tail, current);
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
