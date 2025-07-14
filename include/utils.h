/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:00:16 by apierret          #+#    #+#             */
/*   Updated: 2025/07/14 15:02:20 by apierret         ###   ########.fr       */
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
t_error	split_lst(t_list **lst, char *input, char c);
void	remove_str_quotes(char *str);
int		is_quote(char c);
char	toggle_quote(char c, char current);
void	lst_remove(t_list **lst, int (*f)(void *), void (*del)(void *));
int		str_empty(void *content);
void	close_fd(int fd);
void	close_fds(int fd1, int fd2);
void	close_pipe(int *pipe);
void	close_set(int *fd, int value);
t_error	check_file(char *path, int is_file, int read, int write);
void	sort_array(char **array, int (*cmp)(char c));
char	*prefix_suffix_str(char *str, char *prefix, char *suffix);
int		str_is_blank(char *str);

#endif
