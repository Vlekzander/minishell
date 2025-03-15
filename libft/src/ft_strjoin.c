/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:19:32 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:28:35 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len_s1;
	size_t	len_s2;
	size_t	len_str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	len_str = len_s1 + len_s2;
	str = ft_calloc(len_str +1, sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1, len_s1 +1);
	ft_strlcpy(str + len_s1, s2, len_s2 +1);
	return (str);
}
