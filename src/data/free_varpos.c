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

void	free_varpos(t_varpos *varpos)
{
	if (varpos == NULL)
		return ;
	free(varpos->str);
	free(varpos);
}
