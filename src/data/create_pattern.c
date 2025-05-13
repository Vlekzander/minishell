/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pattern.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:00:18 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 12:38:01 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_pattern	*create_pattern(void)
{
	t_pattern	*pattern;

	pattern = ft_calloc(1, sizeof(t_pattern));
	return (pattern);
}
