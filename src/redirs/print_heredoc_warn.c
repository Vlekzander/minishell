/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_heredoc_warn.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:32:21 by apierret          #+#    #+#             */
/*   Updated: 2025/07/24 14:40:59 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "error.h"
#include "libft.h"
#include "redirs.h"

void	print_heredoc_warn(char *line, char *eof)
{
	if (line != NULL || eof == NULL)
		return ;
	ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
	ft_putstr_fd(": warning: here-document delimited by end-of-file (wanted `",
		STDERR_FILENO);
	ft_putstr_fd(eof, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}
