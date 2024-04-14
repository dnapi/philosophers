/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoil.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:55:53 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/14 06:55:57 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long	ft_atoil(const char *str)
{
	long	sign;
	long	answ;
	long	llong_max;

	llong_max = ((unsigned long)(-1)) / 2;
	sign = 1;
	answ = 0;
	while ((8 < *str && *str < 14) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	while ('0' <= *str && *str <= '9')
	{
		if (answ > llong_max / 10)
			return ((sign > 0) * (-1));
		answ = answ * 10;
		if (answ > llong_max - (*str - '0'))
			return ((sign > 0) * (-1));
		answ = answ + ((*str++) - '0');
	}
	return (sign * answ);
}
