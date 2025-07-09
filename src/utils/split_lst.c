/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:37:37 by apierret          #+#    #+#             */
/*   Updated: 2025/07/09 00:53:13 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "error.h"
#include "libft.h"
#include "utils.h"

static char	*ft_strchr_quote(const char *s, int c)
{
	size_t	i;
	char	ch;
	char	quote;

	if (s == NULL)
		return (NULL);
	ch = (char) c;
	quote = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (quote == 0 && s[i] == ch)
			break ;
		if (is_quote(s[i]))
			quote = handle_quote(s[i], quote);
		i++;
	}
	if (quote == 0 && s[i] == ch)
		return ((char *) &s[i]);
	return (NULL);
}

static t_error	new_node(t_list **node, char *input, size_t size)
{
	char	*content;
	t_list	*ptr;

	if (node == NULL || input == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	content = ft_strndup(input, size);
	if (content == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ptr = ft_lstnew(content);
	if (ptr == NULL)
		return (free(content), error(ERR_ALLOCATION, NULL));
	return (*node = ptr, error(ERR_NONE, NULL));
}

t_error	split_lst(t_list **lst, char *input, char c)
{
	t_list	*list;
	t_list	*node;
	char	*ptr;
	t_error	err;

	if (lst == NULL || input == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	list = NULL;
	while (*input != '\0')
	{
		ptr = ft_strchr_quote(input, c);
		if (ptr == NULL)
			ptr = input + ft_strlen(input);
		if (ptr - input > 0)
		{
			err = new_node(&node, input, ptr - input);
			if (err.id != ERR_NONE)
				return (ft_lstclear(&list, free), err);
			ft_lstadd_back(&list, node);
		}
		input = ptr;
		if (*ptr != '\0')
			input++;
	}
	return (*lst = list, error(ERR_NONE, NULL));
}
