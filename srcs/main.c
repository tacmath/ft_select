#include "ft_select.h"

#define K_UP 4283163
#define K_DOWN 4348699

int oputchar(int c)
{
	return (write(2, &c, 1));
}

int init_struct(t_select **map, int ac, char **av)
{
	int n;

	if (!(*map = ft_memalloc(sizeof(t_select))))
		return (0);
	if (!((*map)->arg = ft_memalloc(sizeof(char*) * ac)))
		return (0);
	if (!((*map)->status = ft_memalloc(sizeof(char) * (ac - 1))))
		return (0);
	n = 0;
	while (++n < ac)
		(*map)->arg[n - 1] = ft_strdup(av[n]);
	(*map)->nb_arg = ac - 1;
	(*map)->nb_co = tgetnum("co");
	(*map)->nb_li = tgetnum("li");
	return (1);
}

void free_struct(t_select *map)
{
	int n;

	n = -1;
	while (map->arg[++n])
		free(map->arg[n]);
	free(map->arg);
	free(map->status);
	free(map);
}

int term_init(void)
{
	struct termios config;

	if (!isatty(0))
		return (0);
	if (tcgetattr(0, &config) < 0)
		return (0);
	config.c_lflag &= ~(ECHO | ICANON);
	config.c_cc[VMIN] = 1;
	config.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &config) < 0)
		return (0);
	if (tgetent(0, getenv("TERM")) < 1)
		return (0);
	tputs(tgetstr("cl", 0), 1, oputchar);
	return (1);
}

int main(int ac, char **av)
{
	t_select *map;
	char	buff[5];

	if (!term_init())
		return (-1);
	if (!init_struct(&map, ac, av))
		return (-1);
	ft_putnbr(map->nb_co);
	ft_putendl("");
	ft_putnbr(map->nb_li);
	ft_putendl("");
	while (1)
	{
		ft_bzero(buff, 5);
		read(0, buff, 4);
		if (*(unsigned int *)buff == K_UP)
			break ;
		else if (*(unsigned int *)buff == K_DOWN)
			break ;
		else
			ft_putendl(buff);
	}
	free_struct(map);
	return (0);
}
