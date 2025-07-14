/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:32:26 by apierret          #+#    #+#             */
/*   Updated: 2025/07/13 21:49:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"
#include "utils.h"

static t_error	lst_cat(char **output, t_list *lst, char *separator)
{
	t_strbuilder	*sb;
	char			*str;

	if (lst == NULL)
		return (*output = NULL, error(ERR_NONE, NULL));
	sb = create_strbuilder(256);
	while (lst != NULL)
	{
		if (!strbuilder_append(sb, lst->content))
			return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
		if (separator != NULL && lst->next != NULL)
		{
			if (!strbuilder_append(sb, separator))
				return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
		}
		lst = lst->next;
	}
	str = ft_strdup(sb->buffer);
	if (str == NULL)
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	return (*output = str, free_strbuilder(sb), error(ERR_NONE, NULL));
}

static t_error	filter_files(t_list **filtered, t_pattern *pattern)
{
	t_list	*files;
	t_error	err;

	if (filtered == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (pattern == NULL)
		return (error(ERR_NONE, NULL));
	err = scan_dir(&files, ".");
	if (err.id != ERR_NONE)
		return (err);
	err = globbing(filtered, files, pattern);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&files, free), err);
	ft_lstclear(&files, free);
	return (error(ERR_NONE, NULL));
}

t_error	expand_wildcard(char **output, char *base, char *mask)
{
	t_pattern	*pattern;
	t_list		*filtered;
	char		*str;
	t_error		err;

	if (output == NULL || base == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	filtered = NULL;
	err = extract_pattern(&pattern, base, mask);
	if (err.id != ERR_NONE)
		return (err);
	err = filter_files(&filtered, pattern);
	if (err.id != ERR_NONE)
		return (free_pattern(pattern), err);
	free_pattern(pattern);
	if (filtered == NULL)
		str = base;
	else
		err = lst_cat(&str, filtered, " ");
	return (ft_lstclear(&filtered, free), *output = str, err);
}
