/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 12:30:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

t_list	*create_token_list(char **strings)
{
	size_t	i;
	t_list	*list;
	t_list	*node;
	t_token	*token;

	list = NULL;
	if (strings == NULL)
		return (NULL);
	i = 0;
	while (strings[i] != NULL)
	{
		token = create_token(get_token_type(strings[i]), strings[i]);
		if (token == NULL)
			return (ft_lstclear(&list, (void *) free_token), NULL);
		node = ft_lstnew(token);
		if (node == NULL)
			return (ft_lstclear(&list, (void *) free_token), NULL);
		ft_lstadd_back(&list, node);
		i++;
	}
	return (list);
}
