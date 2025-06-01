/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:00:16 by apierret          #+#    #+#             */
/*   Updated: 2025/06/01 23:41:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include "error.h"
# include "libft.h"

t_error	str_array_push(char ***array, char *element);
t_error	scan_dir(t_list **content, char *path);
void	free_ddarray(void **array);
char	*ft_strstr(char *str, char *to_find);
char	*ft_strndup(char *src, size_t len);
t_error	split_lst(t_list **lst, char *str, char c);

#endif
