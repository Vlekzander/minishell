/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_arr_equal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:50:46 by apierret          #+#    #+#             */
/*   Updated: 2025/05/06 23:51:04 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <munit.h>
#include <string.h>

#include "test_utils.h"

static int str_equal(char *a, char *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	return (strcmp(a, b) == 0);
}

int	str_array_equal(char **a, char **b)
{
	int	i;

	munit_logf(MUNIT_LOG_WARNING, "%p %p", a, b);
	if (a == NULL || b == NULL)
		return (a == b);
	i = 0;
	while (a[i] != NULL && b[i] != NULL)
	{
		if (!str_equal(a[i], b[i]))
			return (0);
		i++;
	}
	return (a[i] == b[i]);
}
