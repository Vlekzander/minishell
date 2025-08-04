/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:00:16 by apierret          #+#    #+#             */
/*   Updated: 2025/08/01 11:30:24 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include "error.h"
# include "libft.h"

t_error	scan_dir(t_list **content, char *path);
void	free_ddarray(void **array);
char	*ft_strstr(char *str, char *to_find);
char	*ft_strndup(char *src, size_t len);
void	remove_str_quotes(char *str);
int		is_quote(char c);
char	toggle_quote(char c, char current);
void	lst_remove(t_list **lst, int (*f)(void *), void (*del)(void *));
void	close_fd(int fd);
void	close_fds(int fd1, int fd2);
void	close_pipe(int *pipe);
void	close_set(int *fd, int value);
t_error	check_file(char *path, int is_file, int read, int write);
void	sort_array(char **array, int (*cmp)(char c));
int		str_empty(void *content);
int		str_blank(char *str);
int		ptr_null(void *ptr);
int		is_blank(char c);
t_error	lst_array(char ***array, t_list *lst);
void	insert_sublist(t_list *list, t_list *sublist);
t_error	fill_buffer_random(char *buf, size_t len);

#endif
