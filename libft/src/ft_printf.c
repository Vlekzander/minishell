/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:37:36 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 23:06:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>

int	ft_printf_char(char c);
int	ft_printf_str(char *s);
int	ft_printf_ptr(void *ptr);
int	ft_printf_nbr(int n);
int	ft_printf_nbrbase(unsigned long n, char *base);
int	ft_printf_hex(unsigned long n, int is_upper);

static int	ft_print_format(const char *str, int *i, va_list args)
{
	int		ret;
	char	c;

	ret = 0;
	c = str[(*i) + 1];
	if (c == '\0')
		return (1);
	if (c == 'c')
		ret += ft_printf_char(va_arg(args, int));
	else if (c == 's')
		ret += ft_printf_str(va_arg(args, char *));
	else if (c == 'p')
		ret += ft_printf_ptr(va_arg(args, void *));
	else if (c == 'd' || c == 'i')
		ret += ft_printf_nbr(va_arg(args, int));
	else if (c == 'u')
		ret += ft_printf_nbrbase((va_arg(args, unsigned int)), "0123456789");
	else if (c == 'x' || c == 'X')
		ret += ft_printf_hex(va_arg(args, unsigned int), c == 'X');
	else if (c == '%')
		ret += (ft_printf_char('%'));
	if (c == 's' && ret == 0)
		(*i)++;
	(*i) += ret > 0;
	return (ret);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		ret;
	int		printed;

	va_start(args, format);
	ret = 0;
	i = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
			printed = ft_print_format(format, &i, args);
		else
			printed = ft_printf_char(format[i]);
		if (printed == -1)
		{
			va_end(args);
			return (-1);
		}
		ret += printed;
		i++;
	}
	va_end(args);
	return (ret);
}
