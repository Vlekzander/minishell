/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_buffer_random.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 11:29:35 by apierret          #+#    #+#             */
/*   Updated: 2025/08/01 11:30:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "error.h"
#include "redirs.h"
#include "utils.h"

static char	random_to_alphanum(unsigned char r)
{
	r %= 62;
	if (r < 10)
		return ('0' + r);
	if (r < 36)
		return ('a' + r - 10);
	return ('A' + r - 36);
}

t_error	fill_buffer_random(char *buf, size_t len)
{
	int		fd;
	char	c;
	size_t	i;
	t_error	err;

	if (buf == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = open_file(&fd, "/dev/urandom", REDIR_IN, 0);
	if (err.id != ERR_NONE)
		return (err);
	i = 0;
	while (i < len)
	{
		if (read(fd, &c, 1) == -1)
			return (close_fd(fd), error(ERR_ERRNO, NULL));
		buf[i] = random_to_alphanum(c);
		i++;
	}
	return (close_fd(fd), error(ERR_NONE, NULL));
}
