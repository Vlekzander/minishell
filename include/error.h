/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:43:29 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 23:13:43 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H
# define ERROR_PREFIX "minishell"

typedef enum e_error_code
{
	ERR_NONE,
	ERR_ERRNO,
	ERR_IMPLEMENTATION,
	ERR_SYNTAX,
	ERR_ALLOCATION,
	ERR_FILE_NOT_FOUND,
	ERR_PERMISSION,
	ERR_IS_DIRECTORY,
	ERR_PIPE,
	ERR_INVALID_KEY,
	ERR_AMB_REDIR,
	ERR_CMD_NOT_FOUND,
	ERR_FORK,
	ERR_HEREDOC_FILE,
	ERR_NUMERIC_ARG,
	ERR_TOO_MANY_ARGS,
	ERR_HOME_NOT_SET,
	ERR_EXIT,
	ERR_NO_EXEC,
	ERR_WRITE,
	ERR_DUP
}	t_error_code;

typedef struct s_error
{
	t_error_code	id;
	char			*src;
}	t_error;

t_error	error(t_error_code code, char *cause);
void	print_error(t_error err, char *builtin);

#endif