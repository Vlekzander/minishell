/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:51:44 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 18:24:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expansion.h"
#include "utils.h"

static t_error	ignore_file(t_list **files, char *filename)
{
	t_list	*node;
	char	*str;

	if (files == NULL || filename == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = *files;
	while (node != NULL)
	{
		str = node->content;
		if (ft_strncmp(filename, str, ft_strlen(filename) + 1) == 0)
			str[0] = '\0';
		node = node->next;
	}
	lst_remove(files, str_empty, free);
	return (error(ERR_NONE, NULL));
}

static t_error	process_filter_ignored(t_list **files, char *str, char *mask)
{
	t_pattern	*pattern;
	t_error		err;

	if (files == NULL || str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = extract_pattern(&pattern, str, mask);
	if (err.id != ERR_NONE)
		return (err);
	if (pattern != NULL)
		err = globbing(files, pattern, 1);
	else
		err = ignore_file(files, str);
	if (err.id != ERR_NONE)
		return (free_pattern(pattern), free(mask), err);
	free_pattern(pattern);
	return (error(ERR_NONE, NULL));
}

static t_error	filter_ignored(t_list **files, char *glob_ign)
{
	char		**ignored;
	char		*mask;
	size_t		i;
	t_error		err;

	if (files == NULL || glob_ign == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	mask = ft_strdup(glob_ign);
	if (mask == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ft_memset(mask, ' ', ft_strlen(mask));
	ignored = ft_split(glob_ign, ':');
	if (ignored == NULL)
		return (free(mask), error(ERR_ALLOCATION, NULL));
	i = 0;
	while (ignored[i] != NULL)
	{
		err = process_filter_ignored(files, ignored[i], mask);
		if (err.id != ERR_NONE)
			return (free_ddarray((void **) ignored), free(mask), err);
		i++;
	}
	return (free_ddarray((void **) ignored), free(mask), error(ERR_NONE, NULL));
}

static t_error	filter_files(t_list **filtered, t_pattern *pattern,
					char *glob_ign)
{
	t_list	*files;
	t_error	err;

	if (filtered == NULL || pattern == NULL || glob_ign == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = scan_dir(&files, ".");
	if (err.id != ERR_NONE)
		return (err);
	err = globbing(&files, pattern, 0);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&files, free), err);
	err = filter_ignored(&files, glob_ign);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&files, free), err);
	return (*filtered = files, error(ERR_NONE, NULL));
}

t_error	pathname_expansion(t_list **lst, char *str, char *mask, char *glob_ign)
{
	t_pattern	*pattern;
	t_list		*filtered;
	t_error		err;

	if (lst == NULL || str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (glob_ign == NULL)
		glob_ign = ".*";
	err = extract_pattern(&pattern, str, mask);
	if (err.id != ERR_NONE)
		return (err);
	if (pattern == NULL)
		return (*lst = NULL, error(ERR_NONE, NULL));
	err = filter_files(&filtered, pattern, glob_ign);
	if (err.id != ERR_NONE)
		return (free_pattern(pattern), err);
	free_pattern(pattern);
	return (*lst = filtered, error(ERR_NONE, NULL));
}
