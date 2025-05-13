/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_equal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:25:43 by apierret          #+#    #+#             */
/*   Updated: 2025/05/12 12:32:05 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "test_utils.h"

int	token_equal(t_token *a, t_token *b)
{
	size_t	len;
	size_t	len_b;

	if (a == NULL || b == NULL || a->type != b->type)
		return (0);
	if (a->value == NULL && b->value == NULL)
		return (1);
	if (a->value == NULL || b->value == NULL)
		return (0);
	len = ft_strlen(a->value);
	len_b = ft_strlen(b->value);
	if (len_b > len)
		len = len_b;
	return (ft_strncmp(a->value, b->value, len) == 0);
}
