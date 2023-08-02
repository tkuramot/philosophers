/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuramot <tkuramot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:44:56 by tkuramot          #+#    #+#             */
/*   Updated: 2023/08/02 16:12:05 by tkuramot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	handle_overflow(long *nb, int digit, int sign)
{
	if (*nb * sign > LONG_MAX / 10 || (*nb * sign == LONG_MAX / 10
			&& digit > LONG_MAX % 10))
	{
		*nb = LONG_MAX;
		return (1);
	}
	if (*nb * sign < LONG_MIN / 10 || (*nb * sign == LONG_MIN / 10 && digit
			> -(LONG_MIN % 10)))
	{
		*nb = LONG_MIN;
		return (1);
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	nb;

	sign = 1;
	nb = 0;
	while (*str == ' ' || ('\t' <= *str && *str <= '\r'))
		str++;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while ('0' <= *str && *str <= '9')
	{
		if (handle_overflow(&nb, *str - '0', sign))
			break ;
		nb = (nb * 10) + (*str - '0');
		str++;
	}
	return ((int)(nb * sign));
}

size_t	my_min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

size_t	my_max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}