/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:49:46 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 14:14:35 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

t_test_case	*find_case(t_test_case *cases, char *name)
{
	size_t	i;
	if (cases == NULL || name == NULL)
		return (NULL);

	i = 0;
	while (cases[i].name != NULL)
	{
		if (str_equal(cases[i].name, name))
			return (&cases[i]);
		i++;
	}
	return (NULL);
}
