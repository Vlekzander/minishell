/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_array_equal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:13:39 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 23:13:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "test_utils.h"

int	str_array_equal(char **a, char **b)
{
	int	i;

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