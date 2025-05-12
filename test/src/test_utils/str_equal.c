/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_equal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:12:00 by apierret          #+#    #+#             */
/*   Updated: 2025/05/12 11:13:32 by apierret         ###   ########.fr       */
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
