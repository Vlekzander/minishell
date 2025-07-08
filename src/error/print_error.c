/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:13:12 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 11:56:01 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "libft.h"

static char	*get_more_error_message(t_error_code code)
{
	if (code == ERR_FORK)
		return ("Fork failed");
	if (code == ERR_HEREDOC_FILE)
		return ("Open heredoc file failed");
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

void	print_error(t_error err)
{
	if (err.id == ERR_NONE)
		return (free(err.src));
	ft_printf("%s: ", ERROR_PREFIX);
	if (err.src != NULL)
	{
		ft_printf("%s: ", err.src);
		free(err.src);
	}
	ft_printf("%s\n", get_error_message(err.id));
}
