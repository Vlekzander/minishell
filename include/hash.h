/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:38:56 by apierret          #+#    #+#             */
/*   Updated: 2025/06/19 11:59:04 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_H
# define HASH_H
# include "libft.h"
# include "stddef.h"

typedef struct s_hash_node
{
	char				*key;
	void				*content;
}	t_hash_node;

typedef struct s_hash_table
{
	t_list	**buckets;
	size_t	buckets_count;
	size_t	(*hash)(char *key);
	void	(*del)(void *);
}	t_hash_table;

t_hash_table	*htable_init(size_t buckets_count, size_t (*hash)(char *key),
					void (*del)(void *));
void			htable_destroy(t_hash_table *htable);
int				htable_insert(t_hash_table *htable, char *key, void *value);
void			*htable_get(t_hash_table *htable, char *key);
int				htable_remove(t_hash_table *htable, char *key);
int				htable_foreach(t_hash_table *htable,
					int (*f)(char *key, void *content, void *data), void *data);
size_t			htable_hash(char *key);
t_hash_node		*htable_search_node(char *key, t_list *lst);
void			free_hnode(t_hash_node *hnode, void (*del)(void *));
t_hash_node		*create_hnode(char *key, void *value);

#endif
