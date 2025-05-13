/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_pattern_equal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:27:44 by apierret          #+#    #+#             */
/*   Updated: 2025/05/12 14:25:00 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "test_utils.h"

int		glob_pattern_equal(t_pattern *a, t_pattern *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	if (!str_equal(a->prefix, b->prefix))
		return (0);
	if (!str_equal(a->suffix, b->suffix))
		return (0);
	return (lst_equal(a->infixes, b->infixes, (void *) str_equal));
}
