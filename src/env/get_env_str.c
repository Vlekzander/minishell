/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:46:40 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 21:04:23 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "data.h"
#include "env.h"
#include "error.h"
#include "hash.h"

static t_error	lst_cat(char **str, t_list *lst)
{
	t_strbuilder	*sb;

	if (str == NULL || lst == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb = create_strbuilder(256);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	while (lst != NULL)
	{
		if (!strbuilder_append(sb, lst->content))
			return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
		if (!strbuilder_append_char(sb, '\n'))
			return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
		lst = lst->next;
	}
	*str = ft_strdup(sb->buffer);
	if (*str == NULL)
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	return (free_strbuilder(sb), error(ERR_NONE, NULL));
}

t_error	get_env_str(char **str, int export, t_hash_table *env)
{
	char	*str_env;
	t_list	*lst;
	t_error	err;

	if (str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = get_env(&lst, export, env);
	if (err.id != ERR_NONE)
		return (err);
	err = lst_cat(&str_env, lst);
	if (err.id != ERR_NONE)
		return (err);
	return (ft_lstclear(&lst, free), *str = str_env, error(ERR_NONE, NULL));
}
