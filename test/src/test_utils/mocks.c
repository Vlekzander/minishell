/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mocks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:14:10 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 15:00:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

__attribute__((weak))
t_error	__wrap_expand(t_list **tk_list, char **env)
{
	(void) tk_list;
	(void) env;
	return (ERR_NONE);
}
