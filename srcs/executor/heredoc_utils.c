/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 12:00:00 by raisufaj          #+#    #+#             */
/*   Updated: 2026/11/11 12:00:00 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

// 'E"O"F' -> EOF : quotes are removed, nothing is expanded.
char	*strip_quotes(char *s)
{
	char	*res;
	char	quote;
	int		i;
	int		j;

	res = malloc(ft_strlen(s) + 1);
	if (!res)
		return (NULL);
	quote = 0;
	i = 0;
	j = 0;
	while (s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		else
			res[j++] = s[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

// Creates a new, unique temp file. O_EXCL never reuses an existing file.
char	*heredoc_path(int *fd)
{
	char	*num;
	char	*path;
	int		i;

	i = 0;
	while (i < 1000)
	{
		num = ft_itoa(i++);
		if (!num)
			return (NULL);
		path = ft_strjoin(HEREDOC_PREFIX, num);
		free(num);
		if (!path)
			return (NULL);
		*fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (*fd >= 0)
			return (path);
		free(path);
	}
	return (NULL);
}

// Expands $NAME and $? in one heredoc line; quotes stay literal.
// Frees `line`.
char	*expand_heredoc_line(char *line, t_shell *sh)
{
	t_expand_state	state;
	char			*res;
	char			*part;
	int				i;

	state.last_status = sh->last_status;
	state.in_double_quotes = true;
	res = ft_strdup("");
	i = 0;
	while (res && line[i])
	{
		if (line[i] == '$' && (ft_isalpha(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?'))
			part = expand_variable(line, &i, sh->env, &state);
		else
		{
			part = ft_strndup(line + i, 1);
			i++;
		}
		append_string(&res, part);
		free(part);
	}
	free(line);
	return (res);
}
