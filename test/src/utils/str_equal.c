/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_equal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 19:01:49 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 19:02:05 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "test_utils.h"

int str_equal(char *a, char *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	return (strcmp(a, b) == 0);
}
