/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_equal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:58:49 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 16:40:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "test_utils.h"

int	lst_equal(t_list *a, t_list *b, int (*equ)(void *, void *))
{
	while (a != NULL && b != NULL)
	{
		if (!equ(a->content, b->content))
			return (0);
		a = a->next;
		b = b->next;
	}
	return (a == b);
}

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

int str_equal(char *a, char *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	return (strcmp(a, b) == 0);
}

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
