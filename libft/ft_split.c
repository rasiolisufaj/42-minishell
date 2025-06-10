/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:02:29 by raisufaj          #+#    #+#             */
/*   Updated: 2025/05/14 13:51:02 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_word(const char *str, char c)
{
	size_t	i;
	size_t	in_word;
	size_t	word;

	i = 0;
	in_word = 0;
	word = 0;
	while (str[i])
	{
		if (str[i] == c)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			word++;
		}
		i++;
	}
	return (word);
}

static size_t	word_len(const char *s, char delimiter)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != delimiter)
		len++;
	return (len);
}

static void	ft_free_split(char **ptr, int strs)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (i < strs)
		free(ptr[i++]);
	free(ptr);
}

static char	**fill_words(char **ptr, const char *s, char c)
{
	size_t	i;
	size_t	result_index;
	size_t	wd_len;
	size_t	wd_index;

	i = 0;
	result_index = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			wd_len = word_len(&s[i], c);
			ptr[result_index] = malloc((wd_len + 1) * sizeof(char));
			if (!ptr[result_index])
				return (ft_free_split(ptr, result_index), NULL);
			wd_index = 0;
			while (wd_index < wd_len)
				ptr[result_index][wd_index++] = s[i++];
			ptr[result_index++][wd_index] = '\0';
		}
		else
			i++;
	}
	ptr[result_index] = NULL;
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;

	if (!s)
		return (NULL);
	ptr = malloc((ft_count_word(s, c) + 1) * sizeof(char *));
	if (!ptr)
		return (NULL);
	return (fill_words(ptr, s, c));
}
