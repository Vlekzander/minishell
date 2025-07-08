/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:13:12 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 13:21:01 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "libft.h"

static char	*get_more_error_message(t_error_code code)
{
	if (code == ERR_FORK)
		return ("Fork failed");
	if (code == ERR_HEREDOC_FILE)
		return ("Open heredoc file failed");
	if (code == ERR_NUMERIC_ARG)
		return ("Numeric argument required");
	if (code == ERR_TOO_MANY_ARGS)
		return ("Too many arguments");
	if (code == ERR_HOME_NOT_SET)
		return ("HOME not set");
	return ("Unknown error");
}

static char	*get_error_message(t_error_code code)
{
	if (code == ERR_NONE)
		return ("");
	if (code == ERR_ERRNO)
		return (strerror(errno));
	if (code == ERR_IMPLEMENTATION)
		return ("Implementation error");
	if (code == ERR_SYNTAX)
		return ("Syntax error");
	if (code == ERR_ALLOCATION)
		return ("Allocation error");
	if (code == ERR_FILE_NOT_FOUND)
		return ("No such file or directory");
	if (code == ERR_PERMISSION)
		return ("Permission denied");
	if (code == ERR_IS_DIRECTORY)
		return ("Is a directory");
	if (code == ERR_PIPE)
		return ("Pipe failed");
	if (code == ERR_INVALID_KEY)
		return ("Not a valid identifier");
	if (code == ERR_AMB_REDIR)
		return ("Ambiguous redirect");
	if (code == ERR_CMD_NOT_FOUND)
		return ("Command not found");
	return (get_more_error_message(code));
}

void	print_error(t_error err, char *source)
{
	if (err.id == ERR_NONE)
		return (free(err.src));
	ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (source != NULL)
	{
		ft_putstr_fd(source, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (err.src != NULL)
	{
		ft_putstr_fd(err.src, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		free(err.src);
	}
	ft_putendl_fd(get_error_message(err.id), STDERR_FILENO);
}
