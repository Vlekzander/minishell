/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:57:44 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 19:00:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "test_utils.h"

static size_t	cases_len(t_case *cases)
{
	size_t	size;

	size = 0;
	while (cases[size].name != NULL)
		size++;
	return (size);
}

t_case	*find_case(t_case *cases, const char *name)
{
	size_t	size;
	size_t	i;

	size = cases_len(cases);
	i = 0;
	while (i < size)
	{
		if (strcmp(cases[i].name, name) == 0)
			return (&cases[i]);
		i++;
	}
	return (NULL);
}
