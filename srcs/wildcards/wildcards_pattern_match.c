/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_pattern_match.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 00:50:00 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/12 00:53:33 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	handle_star_pattern(char **p, char **n, char **star_p, char **star_n)
{
	if (**p == '*')
	{
		*star_p = (*p)++;
		*star_n = *n;
		return (1);
	}
	else if (**p == **n)
	{
		(*p)++;
		(*n)++;
		return (1);
	}
	else if (*star_p)
	{
		*p = *star_p + 1;
		*n = ++(*star_n);
		return (1);
	}
	return (0);
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
		if (!handle_star_pattern(&p, &n, &star_p, &star_n))
			return (0);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}

int	should_include_entry(char *pattern, struct dirent *entry)
{
	return (ft_strcmp(entry->d_name, ".") != 0
		&& ft_strcmp(entry->d_name, "..") != 0
		&& match_patt(pattern, entry->d_name)
		&& (pattern[0] == '.' || entry->d_name[0] != '.'));
}

static bool	token_is_quoted(const char *value)
{
	return (ft_strchr(value, '\'') != NULL || ft_strchr(value, '"') != NULL);
}

bool	should_expand_wildcard(t_token *token)
{
	return (token->type == T_WORD && !token_is_quoted(token->value)
		&& ft_strchr(token->value, '*'));
}
