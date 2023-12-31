/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuramot <tkuramot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 21:42:57 by tkuramot          #+#    #+#             */
/*   Updated: 2023/08/19 13:13:42 by tkuramot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	pick_up_forks(t_philo *philo, int64_t r_fork, int64_t l_fork)
{
	pthread_mutex_lock(&philo->share->forks[my_min(r_fork, l_fork)]);
	print_philo_state(philo, M_TAKEN_FORK);
	if (philo->config->nbr_philos == 1)
	{
		my_usleep(philo->config->time_to_die * 2000);
		return ;
	}
	pthread_mutex_lock(&philo->share->forks[my_max(r_fork, l_fork)]);
	print_philo_state(philo, M_TAKEN_FORK);
}

static void	drop_forks(t_philo *philo, int64_t r_fork, int64_t l_fork)
{
	pthread_mutex_unlock(&philo->share->forks[my_max(r_fork, l_fork)]);
	pthread_mutex_unlock(&philo->share->forks[my_min(r_fork, l_fork)]);
}

void	philo_eat(t_philo *philo)
{
	int64_t			r_fork;
	int64_t			l_fork;
	struct timeval	t;

	l_fork = philo->id;
	r_fork = (philo->id + 1) % philo->config->nbr_philos;
	pick_up_forks(philo, r_fork, l_fork);
	update_last_meal_time(philo);
	gettimeofday(&t, NULL);
	t.tv_sec += philo->config->time_to_eat / 1000;
	t.tv_usec += philo->config->time_to_eat * 1000;
	sleep_until(&t);
	drop_forks(philo, r_fork, l_fork);
	philo->nbr_meals++;
	if (philo->config->nbr_meals != -1
		&& philo->nbr_meals >= philo->config->nbr_meals)
	{
		pthread_mutex_lock(&philo->share->lock_share);
		if (!philo->is_satisfied)
			philo->share->nbr_satisfied_philos++;
		philo->is_satisfied = true;
		pthread_mutex_unlock(&philo->share->lock_share);
	}
}

void	philo_sleep(t_philo *philo)
{
	struct timeval	t;

	print_philo_state(philo, M_SLEEPING);
	gettimeofday(&t, NULL);
	t.tv_sec += philo->config->time_to_sleep / 1000;
	t.tv_usec += philo->config->time_to_sleep * 1000;
	sleep_until(&t);
}

void	philo_think(t_philo *philo)
{
	struct timeval	t;
	int64_t			interval;

	print_philo_state(philo, M_THINKING);
	gettimeofday(&t, NULL);
	t.tv_sec += philo->config->time_to_eat / 1000;
	t.tv_usec += philo->config->time_to_eat * 1000;
	if (philo->nbr_meals == 0)
	{
		sleep_until(&philo->next_meal);
		return ;
	}
	else
	{
		t = philo->last_meal;
		interval = philo->config->time_to_die - 50;
		t.tv_sec += interval / 1000;
		t.tv_usec += interval * 1000;
		sleep_until(&t);
	}
}
