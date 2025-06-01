/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:43:04 by apierret          #+#    #+#             */
/*   Updated: 2025/05/29 23:46:42 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_strndup(char *src, size_t len)
{
	char	*str;

	if (src == NULL)
		return (NULL);
	str = ft_calloc(len +1, sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, src, len +1);
	return (str);
}
