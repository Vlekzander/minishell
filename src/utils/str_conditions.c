/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cond_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:38:14 by apierret          #+#    #+#             */
/*   Updated: 2025/07/14 17:39:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

int	str_empty(void *content)
{
	char	*str;

	str = content;
	return (ft_strlen(str) == 0);
}

int	str_blank(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	ptr_null(void *ptr)
{
	return (ptr == NULL);
}
