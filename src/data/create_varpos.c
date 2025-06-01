/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_varpos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:10:43 by apierret          #+#    #+#             */
/*   Updated: 2025/06/01 20:12:35 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_varpos	*create_varpos(void)
{
	t_varpos	*varpos;

	varpos = ft_calloc(1, sizeof(t_varpos));
	if (varpos == NULL)
		return (NULL);
	return (varpos);
}
