/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_varpos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:10:43 by apierret          #+#    #+#             */
/*   Updated: 2025/06/03 16:09:20 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_vref	*create_vref(void)
{
	t_vref	*vref;

	vref = ft_calloc(1, sizeof(t_vref));
	if (vref == NULL)
		return (NULL);
	return (vref);
}
