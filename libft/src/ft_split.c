/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:11:44 by apierret          #+#    #+#             */
/*   Updated: 2024/10/03 16:46:53 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static size_t	ft_countwords(char const *s, char c)
{
	size_t	count;
	size_t	i;
	int		in_word;

	count = 0;
	in_word = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (!in_word && s[i] != c)
		{
			in_word = 1;
			count++;
		}
		if (in_word && s[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

static void	free_strings(char **strs)
{
	size_t	i;

	if (strs == NULL)
		return ;
	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static char	*allocate_word(const char *s, char c, size_t *i)
{
	size_t	start;
	size_t	len;

	while (s[*i] && s[*i] == c)
		(*i)++;
	start = *i;
	while (s[*i] && s[*i] != c)
		(*i)++;
	len = *i - start;
	return (ft_substr(s, start, len));
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	size_t	i;
	size_t	j;

	if (s == NULL)
		return (NULL);
	i = ft_countwords(s, c);
	strs = (char **) malloc(sizeof(char *) * (i +1));
	if (strs == NULL)
		return (NULL);
	strs[i] = NULL;
	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			strs[j] = allocate_word(s, c, &i);
			if (strs[j++] == NULL)
				return (free_strings(strs), NULL);
		}
		else
			i++;
	}
	return (strs);
}
