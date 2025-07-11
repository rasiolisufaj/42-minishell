/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_directory.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 00:50:00 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 01:11:23 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

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
			new_token = create_token_copy(entry->d_name);
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

t_token	*handle_star_slash_pattern(char *pattern)
{
	t_token		*dirs;
	t_token		*head;
	t_token		*current;
	t_token		*files;
	struct stat	statbuf;

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
