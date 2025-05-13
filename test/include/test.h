/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:33:03 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 23:39:23 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H
# include <setjmp.h>
# include <stdarg.h>
# include <stddef.h>
# include <cmocka.h>
# include "error.h"
# include "libft.h"
# define SUCCESS_MSG "\t[  \x1b[32mOK\x1b[0m  ] %s\n"
# define FAIL_MSG "\t[ \x1b[31mFAIL\x1b[0m ] %s\n"
# define CASE_NOT_FOUND_MSG "\t[ \x1b[31mFAIL\x1b[0m ] case \"%s\" not found\n"
# define GROUP_HEADER "[  START  ] %s\n"
# define GROUP_RESULT "[   END   ] %s - %d/%d tests passed\n"
# define GLOBAL_RESULt "\n[ \x1b[33mRESULTS\x1b[0m ] %d/%d tests passed\n"

typedef struct s_test_case {
	char	*name;
	union
	{
		struct
		{
			char	*input_tokenize;
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
			char	*input_pattern;
			void	*excepted_pattern;
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
}	t_test_case;

typedef struct s_test_result
{
	int	total;
	int	failed;
	int successful;
} t_test_result;

t_test_result	tokenize_tests(void);
t_test_result	globbing_tests(void);
t_test_result	extract_pattern_tests(void);
t_test_result	parse_ast_tests(void);

#endif
