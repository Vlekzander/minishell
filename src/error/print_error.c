/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:44:04 by apierret          #+#    #+#             */
/*   Updated: 2025/03/24 15:44:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"

static char	*error_message(t_error error)
{
	if (error == ERR_NONE)
		return ("No error occurred.");
	if (error == ERR_IMPLEMENTATION)
		return ("Implementation error, are you serious?");
	return ("Unknown error");
}

void	print_error(char *source, t_error error)
{
	char	*message;

	message = error_message(error);
	if (source != NULL)
	{
		ft_putstr_fd(source, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
}
