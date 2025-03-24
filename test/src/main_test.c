/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:45 by apierret          #+#    #+#             */
/*   Updated: 2025/03/24 16:41:38 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "munit.h"

static int addition(int a, int b)
{
	return (a + b);
}

static MunitResult test_addition_positive(const MunitParameter params[], void* data)
{
  	(void) params;
  	(void) data;
	munit_assert_int(addition(2, 3), ==, 5);
	return (MUNIT_OK);
}

static MunitResult test_addition_zero(const MunitParameter params[], void* data)
{
	(void) params;
	(void) data;
	munit_assert_int(addition(0, 0), ==, 0);
	return (MUNIT_OK);
}

static MunitResult test_addition_negatif(const MunitParameter params[], void* data)
{
	(void) params;
	(void) data;
	munit_assert_int(addition(-2, -3), ==, -5);
	return (MUNIT_OK);
}

static MunitTest tests[] = {
	{ "/addition/positive", test_addition_positive, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/addition/zero",     test_addition_zero,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/addition/negatif",  test_addition_negatif,  NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite math_utils = {
	"/math-utils",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[])
{
	return (munit_suite_main(&math_utils, NULL, argc, argv));
}
