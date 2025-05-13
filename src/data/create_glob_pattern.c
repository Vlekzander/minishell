/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_glob_pattern.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:00:18 by apierret          #+#    #+#             */
/*   Updated: 2025/05/12 15:02:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_glob_pattern	*create_glob_pattern(void)
{
	t_glob_pattern	*pattern;

	pattern = ft_calloc(1, sizeof(t_glob_pattern));
	return (pattern);
}
