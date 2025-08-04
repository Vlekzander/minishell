/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:01 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 20:28:50 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "env.h"
#include "utils.h"

static int	add_env_to_lst(char *key, void *value, void *data)
{
	t_list			**lst;
	size_t			len;
	char			*str;
	t_list			*node;

	if (key == NULL || data == NULL || data + 1 == NULL)
		return (0);
	if (value == NULL || ft_strncmp("?", key, 2) == 0)
		return (1);
	lst = data;
	len = ft_strlen(key) + ft_strlen(value) +1;
	str = ft_calloc(len + 1, sizeof(char));
	if (str == NULL)
		return (0);
	node = ft_lstnew(str);
	if (node == NULL)
		return (free(str), 0);
	ft_strlcat(str, key, len + 1);
	ft_strlcat(str, "=", len + 1);
	ft_strlcat(str, value, len + 1);
	ft_lstadd_back(lst, node);
	return (1);
}

static int	add_export_to_lst(char *key, void *value, void *data)
{
	t_list			**lst;
	size_t			len;
	char			*str;
	t_list			*node;

	if (key == NULL || data == NULL)
		return (0);
	if (ft_strncmp("?", key, 2) == 0 || ft_strncmp("_", key, 2) == 0)
		return (1);
	lst = data;
	len = ft_strlen(key) + ft_strlen(value) +1 + ft_strlen("declare -x \"\"");
	str = ft_calloc(len + 1, sizeof(char));
	if (str == NULL)
		return (0);
	node = ft_lstnew(str);
	if (node == NULL)
		return (free(str), 0);
	ft_strlcat(str, "declare -x ", len + 1);
	ft_strlcat(str, key, len + 1);
	if (value == NULL)
		return (ft_lstadd_back(lst, node), 1);
	ft_strlcat(str, "=\"", len + 1);
	ft_strlcat(str, value, len + 1);
	ft_strlcat(str, "\"", len + 1);
	return (ft_lstadd_back(lst, node), 1);
}

t_error	get_env(t_list **lst, int export, t_hash_table *env)
{
	t_list			*list;
	t_error			err;

	if (lst == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	list = NULL;
	if (export)
	{
		if (!htable_foreach(env, add_export_to_lst, &list))
			return (ft_lstclear(&list, free), error(ERR_ALLOCATION, NULL));
		err = lst_sort(list, (void *) ft_strncmp);
		if (err.id != ERR_NONE)
			return (ft_lstclear(&list, free), err);
	}
	else if (!htable_foreach(env, add_env_to_lst, &list))
		return (ft_lstclear(&list, free), error(ERR_ALLOCATION, NULL));
	return (*lst = list, error(ERR_NONE, NULL));
}
