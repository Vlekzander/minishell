/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 12:00:53 by apierret          #+#    #+#             */
/*   Updated: 2024/10/12 17:48:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"

static char	*extract_next_line(char *buffer, int size,
	int end_line, int is_last)
{
	char	*str;

	if (buffer == NULL)
		return (NULL);
	str = (char *) malloc(sizeof(char) * (end_line +1 + (!is_last)));
	if (str == NULL)
		return (free(buffer), NULL);
	ft_memset(str, '\0', sizeof(char) * (end_line +1 + (!is_last)));
	ft_memmove(str, buffer, end_line +1);
	if (!is_last)
	{
		ft_memmove(buffer, buffer + end_line +1, size - end_line -1);
		ft_memset(buffer + size - end_line -1, '\0', end_line +1);
	}
	else
		buffer[0] = '\0';
	return (str);
}

static int	ft_read_next(int fd, char **buffer)
{
	char	*read_buffer;
	char	*str;
	int		bytes;

	read_buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE +1);
	if (read_buffer == NULL)
		return (-1);
	ft_memset(read_buffer, '\0', BUFFER_SIZE +1);
	bytes = read(fd, read_buffer, BUFFER_SIZE);
	str = ft_strjoin(*buffer, read_buffer);
	if (str == NULL)
		return (free(read_buffer), -1);
	free(*buffer);
	*buffer = str;
	free(read_buffer);
	return (bytes);
}

static int	init_and_conditions(int fd, char **buffer)
{
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		return (free(*buffer), *buffer = NULL, 0);
	}
	if (*buffer == NULL)
	{
		*buffer = (char *) malloc(sizeof(char) * (BUFFER_SIZE +1));
		if (*buffer == NULL)
			return (0);
		ft_memset(*buffer, '\0', sizeof(char) * (BUFFER_SIZE +1));
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*end_line;
	int			len;
	int			bytes;

	if (!init_and_conditions(fd, &buffer))
		return (NULL);
	len = ft_strlen(buffer);
	end_line = ft_strchr(buffer, '\n');
	while (end_line == NULL)
	{
		bytes = ft_read_next(fd, &buffer);
		len = ft_strlen(buffer);
		if (bytes == -1 || (bytes == 0 && len == 0))
			return (free(buffer), buffer = NULL, NULL);
		if (bytes == 0)
			return (extract_next_line(buffer, len, len, 1));
		end_line = ft_strchr(buffer, '\n');
	}
	return (extract_next_line(buffer, len, (end_line - buffer), 0));
}
