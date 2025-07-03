/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:43:29 by apierret          #+#    #+#             */
/*   Updated: 2025/07/02 01:01:57 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef enum e_error_code
{
	ERR_NONE,
	ERR_ERRNO,
	ERR_IMPLEMENTATION,
	ERR_SYNTAX,
	ERR_ALLOCATION,
	ERR_OPEN,
	ERR_CLOSE,
	ERR_FILE_NOT_FOUND,
	ERR_PERMISSION,
	ERR_IS_DIRECTORY,
	ERR_PIPE,
	ERR_INVALID_KEY,
	ERR_AMB_REDIR,
	ERR_CMD_NOT_FOUND
}	t_error_code;

typedef struct s_error
{
	t_error_code	id;
	char			*cause;
}	t_error;

t_error	error(t_error_code code, char *cause);

#endif