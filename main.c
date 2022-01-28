#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

static long	timeval_to_ms(struct timeval timeval)
{
	return ((long)timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

long	get_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval_to_ms(timeval));
}

void error(char *str)
{
	printf("%s", str);
	exit(1);
}

enum activity {
	EATING,
	THINKING,
	SLEEPING,
	NONE
};

struct philo {
	long last_ate;
	int activity;
	int activity_started;
};

int main(int argc, char **argv)
{
	int nr_philos;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int nr_times_to_eat;
	struct philo *philos;
	long time;
	int philo_i;
	char activity[100];
	int	err;
	int	i;
	struct philo *philo;
	int running = 1;
	long started;
	setbuf(stdout, NULL);
	if (argc < 5 || argc > 6)
		error("Provide this tester with the same arguments as philo.\n");
	nr_philos = atoi(argv[1]);
	time_to_die = atoi(argv[2]);
	time_to_eat = atoi(argv[3]);
	time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		nr_times_to_eat = atoi(argv[5]);
	else
		nr_times_to_eat = -1;
	philos = calloc(sizeof(*philos), nr_philos);
	for (i = 0; i < nr_philos; i++) {
		philos[i].activity = NONE;
	}


	started = get_time();
	while(running)
	{
		if (get_time() > started + 3000)
			running = 0;
		err = fscanf(stdin, "%ld %d %s", &time, &philo_i, activity);
		if (err == -1)
			break;
		if (err != 3)
		{
			printf("error parsing input, found only %d arguments!\n", err);
			exit(1);
		}
		philo = philos + philo_i;
		if (!strcmp("is", activity))
		{
			err = fscanf(stdin, "%s\n", activity);
			if (err == -1)
				break;
			if (err != 1)
			{
				printf("error parsing input, found no activity!\n");
				exit(1);
			}
		}
		else if (!strcmp("died", activity))
			running = 0;
		else if (!strcmp("has", activity))
		{
			err = fscanf(stdin, "%s %s %s\n", activity, activity, activity);
		}
		else {
			printf("error parsing input, don't understand %s\n", activity);
			exit(1);
		}
		//printf("time %ld, philo %d, activity: %s\n", time, philo_i, activity);
		if (philo->activity == NONE)
			philo->last_ate = time;
		if (!strcmp("thinking", activity))
		{
			philo->activity = THINKING;
			philo->activity_started = time;
		}
		if (!strcmp("eating", activity))
		{
			philo->activity = EATING;
			philo->activity_started = time;
			philo->last_ate= time;
		}
		if (!strcmp("sleeping", activity))
		{
			philo->activity = SLEEPING;
			philo->activity_started = time;
		}
		for (i = 0; i < nr_philos; i++)
		{
			philo = philos + i;
			if (philo->activity == NONE)
				continue;
			if (time > philo->last_ate + time_to_die + 10)
			{
				printf("philosopher %d should have died at %ld, but it's now %ld\n", i, philo->last_ate + time_to_die, time);
				exit(1);
			}
		}
	}
	printf("They all survived for 3 seconds!\n");
}






