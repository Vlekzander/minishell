/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:37:01 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:49:34 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include "utils.h"

static void	lst_swap_content(t_list *a, t_list *b)
{
	void	*tmp;

	if (a == NULL || b == NULL)
		return ;
	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
}

t_error	lst_sort(t_list *lst, int (*cmp)(char *s1, char *s2, size_t size))
{
	t_list	*a;
	t_list	*b;
	size_t	a_len;

	if (lst == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	a = lst;
	while (a != NULL)
	{
		a_len = ft_strlen(a->content);
		b = a->next;
		while (b)
		{
			if (cmp(a->content, b->content, a_len +1) > 0)
				lst_swap_content(a, b);
			b = b->next;
		}
		a = a->next;
	}
	return (error(ERR_NONE, NULL));
}
