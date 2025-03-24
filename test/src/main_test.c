/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:45 by apierret          #+#    #+#             */
/*   Updated: 2025/03/24 23:58:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"
#include "munit.h"
#include "test_utils.h"

static MunitResult	tokenize_test(const MunitParameter params[], void* data)
{
	(void) params;
	(void) data;
	t_list	*list = NULL;
	t_list	*tested = NULL;
	ft_lstadd_back(&list, ft_lstnew(&(t_token){TK_WORD, "Hello"}));
	ft_lstadd_back(&list, ft_lstnew(&(t_token){TK_WORD, "World"}));
	tokenize(&tested, "Hello World");
	munit_assert_true(lst_equal(list, tested, token_equal));
	//TODO Clean tested
	return (MUNIT_OK);
}

static MunitTest tests[] = {
	{"/lexer/tokenize/helloworld", tokenize_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
	"",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[])
{
	return (munit_suite_main(&suite, NULL, argc, argv));
}
