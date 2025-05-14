/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_equal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:58:03 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 19:03:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

int	token_equal(t_token *a, t_token *b)
{
	if (a == NULL || b == NULL || a->type != b->type)
		return (0);
	if (a->value == NULL && b->value == NULL)
		return (1);
	if (a->value == NULL || b->value == NULL)
		return (0);
	return (str_equal(a->value, b->value));
}
