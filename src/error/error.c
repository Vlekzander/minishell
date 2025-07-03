/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:29:23 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:44:03 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"

t_error	error(t_error_code code, char *cause)
{
	t_error	err;

	err.id = code;
	err.src = ft_strdup(cause);
	return (err);
}
