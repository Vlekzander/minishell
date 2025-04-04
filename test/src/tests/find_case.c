/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:01:03 by apierret          #+#    #+#             */
/*   Updated: 2025/04/04 12:20:36 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "tests.h"

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
	size_t size = cases_len(cases);
	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(cases[i].name, name) == 0)
			return (&cases[i]);
	}
	return (NULL);
}
