/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:57:54 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 19:03:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

t_list	*create_token_list(char **strings)
{
	size_t	i;
	t_list	*list = NULL;
	t_token	*token;

	i = 0;
	while (strings[i] != NULL)
	{
		token = create_token(get_token_type(strings[i]), strings[i]);
		ft_lstadd_back(&list, ft_lstnew(token));
		i++;
	}
	return (list);
}
