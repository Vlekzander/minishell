/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_equal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:35:11 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 23:35:24 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "test_utils.h"

int		pattern_equal(t_pattern *a, t_pattern *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	if (!str_equal(a->prefix, b->prefix))
		return (0);
	if (!str_equal(a->suffix, b->suffix))
		return (0);
	return (lst_equal(a->infixes, b->infixes, (void *) str_equal));
}
