/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_varpos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:11:27 by apierret          #+#    #+#             */
/*   Updated: 2025/06/01 20:22:47 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"

void	free_vref(t_vref *vref)
{
	if (vref == NULL)
		return ;
	free(vref->str);
	free(vref);
}
