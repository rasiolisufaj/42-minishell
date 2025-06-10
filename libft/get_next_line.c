/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:26:09 by raisufaj          #+#    #+#             */
/*   Updated: 2025/05/14 14:07:22 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*fill_line_buffer(int fd, char *remainder, char *buffer);
static char	*set_line(char *line_buffer);

char	*get_next_line(int fd)
{
	static char	*remainder[MAX_FD];
	char		*line;
	char		*buffer;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(remainder[fd]);
		free(buffer);
		remainder[fd] = NULL;
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	line = fill_line_buffer(fd, remainder[fd], buffer);
	free(buffer);
	buffer = NULL;
	if (!line)
		return (NULL);
	remainder[fd] = set_line(line);
	return (line);
}

static	char	*set_line(char *line_buffer)
{
	char	*remainder;
	ssize_t	i;

	i = 0;
	while (line_buffer[i] != '\n' && line_buffer[i] != '\0')
		i++;
	if (line_buffer[i] == '\0' || line_buffer[i + 1] == '\0')
		return (NULL);
	remainder = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - i);
	if (*remainder == 0)
	{
		free(remainder);
		remainder = NULL;
	}
	line_buffer[i + 1] = 0;
	return (remainder);
}

static	char	*fill_line_buffer(int fd, char *remainder, char *buffer)
{
	ssize_t	bytes_read;
	char	*tmp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(remainder);
			return (NULL);
		}
		else if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!remainder)
			remainder = ft_strdup("");
		tmp = remainder;
		remainder = ft_strjoin(tmp, buffer);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (remainder);
}
