/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:41:52 by apierret          #+#    #+#             */
/*   Updated: 2025/05/07 17:20:44 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <munit.h>
#include <stdlib.h>
#include "data.h"
#include "lexer.h"

static char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	if (to_find[0] == '\0')
		return (str);
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i + j] != '\0' && str[i + j] == to_find[j])
		{
			if (to_find[j +1] == '\0')
				return (str + i);
			j++;
		}
		i++;
	}
	return (NULL);
}

static t_error	lst_to_array(char ***out, t_list *in)
{
	size_t	i;
	t_list	*cursor;

	if (out == NULL)
		return (ERR_IMPLEMENTATION);
	i = 0;
	cursor = in;
	while (cursor != NULL)
	{
		if (cursor->content != NULL)
			i++;
		cursor = cursor->next;
	}
	*out = ft_calloc(i +1, sizeof(char *));
	i = 0;
	while (in != NULL)
	{
		if (in->content != NULL)
			(*out)[i++] = ft_strdup(in->content);
		in = in->next;
	}
	return (ERR_NONE);
}

t_error	globbing(char ***out_files, char **in_files, t_glob *patterns)
{
	t_list	*files;
	t_list	*node;
	t_list	*infixe;
	t_error	error;
	char	*ptr;

	if (out_files == NULL || in_files == NULL || patterns == NULL)
		return (ERR_IMPLEMENTATION);
	*out_files = NULL;
	files = NULL;
	while (*in_files != NULL)
	{
		node = ft_lstnew(*in_files);
		if (node == NULL)
			return (ft_lstclear(&files, NULL), ERR_ALLOCATION);
		ft_lstadd_back(&files, node);
		in_files++;
	}
	node = files;
	while (node != NULL)
	{
		ptr = node->content;
		if (patterns->prefix != NULL)
		{
			ptr = ft_strstr(ptr, patterns->prefix);
			if (ptr != node->content)
				node->content = NULL;
		}
		if (patterns->infixes != NULL && node->content != NULL)
		{
			infixe = patterns->infixes;
			while (infixe != NULL)
			{
				ptr = ft_strstr(ptr, infixe->content);
				if (ptr == NULL)
				{
					node->content = NULL;
					break ;
				}
				infixe = infixe->next;
			}
		}
		if (patterns->suffix != NULL && node->content != NULL)
		{
			ptr = ft_strstr(ptr, patterns->suffix);
			if (ft_strlen(ptr) != ft_strlen(patterns->suffix))
				node->content = NULL;
		}
		node = node->next;
	}
	error = lst_to_array(out_files, files);
	return (ft_lstclear(&files, NULL), error);
}
