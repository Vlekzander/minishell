/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:34:27 by apierret          #+#    #+#             */
/*   Updated: 2025/05/07 21:34:50 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef TESTS_H
# define TESTS_H
# include "data.h"
# include "error.h"
# include "munit.h"

typedef struct s_case
{
	char	*name;
	union
	{
		struct
		{
			char	*input;
			char	*expected_tokens[100];
		};
		struct
		{
			t_list	*in_files;
			void	*patterns;
			t_list	*excepted_out_files;
		};
		struct
		{
			char	*tokens[100];
			union
			{
				void	*excepted_ast;
				t_error	excepted_error;
			};
		};
	};
} t_case;

t_case	*find_case(t_case cases[], const char *name);
MunitResult	tokenize_basic_tests(const MunitParameter params[], void* data);
MunitResult	globbing_basic_tests(const MunitParameter params[], void* data);
MunitResult	parse_ast_basic_tests(const MunitParameter params[], void* data);
MunitResult	parse_ast_error_tests(const MunitParameter params[], void* data);

#endif
