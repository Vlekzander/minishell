/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorq.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:43:09 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 11:43:48 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

t_error	errorq(t_error_code code, char *cause)
{
	t_error	err;

	err = error(code, cause);
	err.exit = 1;
	return (err);
}
