/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:13:12 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 12:28:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "libft.h"

static char	*get_error_message(t_error_code code)
{
	if (code == ERR_NONE)
		return "";
	if (code == ERR_ERRNO)
		return (strerror(errno));
	if (code == ERR_IMPLEMENTATION)
		return "Implementation error";
	if (code == ERR_SYNTAX)
		return "Syntax error";
	if (code == ERR_ALLOCATION)
		return "Allocation error";
	if (code == ERR_FILE_NOT_FOUND)
		return "No such file or directory";
	if (code == ERR_PERMISSION)
		return "Permission denied";
	if (code == ERR_IS_DIRECTORY)
		return "Is a directory";
	if (code == ERR_PIPE)
		return "Pipe failed";
	if (code == ERR_INVALID_KEY)
		return "Not a valid identifier";
	if (code == ERR_AMB_REDIR)
		return "Ambigous redirect";
	if (code == ERR_CMD_NOT_FOUND)
		return "Command not found";
	return "Unknown error";
}
void	print_error(t_error err)
{
	if (err.id == ERR_NONE)
		return (free(err.cause));
	ft_printf("%s: ", ERROR_PREFIX);
	if (err.cause != NULL)
	{
		ft_printf("%s: ", err.cause);
		free(err.cause);
	}
	ft_printf("%s\n", get_error_message(err.id));
}
