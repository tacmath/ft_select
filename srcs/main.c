#include "ft_select.h"

#define K_UP 4283163
#define K_DOWN 4348699
#define K_RIGHT 4414235
#define K_LEFT 4479771
#define K_SPACE 32
#define K_RETURN 10
#define K_REMOVE 2117294875
#define K_BACKSPACE 127
#define K_ESCAPE 27


int oputchar(int c)
{
	return (write(2, &c, 1));
}

int init_struct(t_select **map, int ac, char **av, struct termios old)
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
	{
		(*map)->arg[n - 1] = ft_strdup(av[n]);
		if ((int)ft_strlen(av[n]) > (*map)->longest_arg)
			(*map)->longest_arg = ft_strlen(av[n]); 
	}
	(*map)->old_term = old;
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

void clean_exit(t_select *map)
{
	tputs(tgetstr("te", 0), 1, oputchar);
	tcsetattr(0, TCSANOW, &map->old_term);
	free_struct(map);
	exit(0);	
}

t_select *get_map(t_select *map)
{
	static t_select *tmp = 0;	
	
	if (!tmp)
		tmp = map;
	return (tmp);
}

int term_init(struct termios *old)
{
	struct termios config;

	if (!isatty(0))
		return (0);
	if (tcgetattr(0, &config) < 0 || tcgetattr(0, old) < 0)
		return (0);
	config.c_lflag &= ~(ECHO | ICANON);
	config.c_cc[VMIN] = 1;
	config.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &config) < 0)
		return (0);
	if (tgetent(0, getenv("TERM")) < 1)
		return (0);
	tputs(tgetstr("ti", 0), 1, oputchar);
	return (1);
}

void move_cursor(t_select *map, int nb)
{
	int apl;
	int nb_line;
	int nb_co;

	apl = (map->nb_co - 1) / (map->longest_arg + 1);
	nb_line = nb / apl + 2;
	nb_co = (nb % apl) * (map->longest_arg + 1);
	tputs(tgoto(tgetstr("cm", 0), nb_co, nb_line), 1, oputchar);
}

void display_one_arg(t_select *map, int nb)
{
	int len;	
	
	len = ft_strlen(map->arg[nb]);
	if (map->status[nb])
		ft_putstr(_REVERSE_);
	if (nb == map->cursor)
		ft_putstr(_UNDERLINED_);
	write(0, map->arg[nb], len--);
	ft_putstr(_EOFORMAT_ _EOC_);
	while (len++ < map->longest_arg)
		ft_putchar_fd(' ', 0);
}

int display_all(t_select *map)
{
	int n;
	int m;
	int ret;

	tputs(tgetstr("cl", 0), 1, oputchar);
	tputs(tgetstr("vi", 0), 1, oputchar);
	write(0, "ft_select :\n", 12);
	ft_putchar('\n');
	n = -1;
	m = 0;
	ret = 0;
	while (map->arg[++n])
	{
		m++;
		if (m * (map->longest_arg + 1) >= map->nb_co - 1)
		{	
			ft_putchar_fd('\n', 0);
			m = 1;	
		}
		display_one_arg(map, n);
		if (n == map->cursor)
			ret = n;
	}
	move_cursor(map, ret);
	tputs(tgetstr("ve", 0), 1, oputchar);
	return (1);
}

void remove_arg(t_select *map)
{
	int n;

	n = map->cursor;
	free(map->arg[n]);
	while (++n < map->nb_arg)
	{
		map->arg[n - 1] = map->arg[n];
		map->status[n - 1] = map->status[n];
	}
	map->arg[n - 1] = 0;
	map->nb_arg--;
	if (map->nb_arg == 0)
		clean_exit(map);
	if (map->nb_arg == map->cursor)
		map->cursor--;
}

void return_choice(t_select *map)
{
	int n;

	tputs(tgetstr("te", 0), 1, oputchar);
	tcsetattr(0, TCSANOW, &map->old_term);
	n = -1;
	while (map->arg[++n])
		if (map->status[n])
		{
			ft_putstr(map->arg[n]);	
			ft_putchar(' ');
		}
	free_struct(map);
	exit(0);	
}

int get_controls(t_select *map)
{
	long int	buf;
	
	buf = 0;
	read(0, &buf, 7);
	if (buf == K_UP)
		map->cursor -= (map->nb_co - 1) / (map->longest_arg + 1);
	else if (buf == K_DOWN)
		map->cursor += (map->nb_co - 1) / (map->longest_arg + 1);
	else if (buf == K_RIGHT)
		map->cursor++;
	else if (buf == K_LEFT)
		map->cursor--;
	if (map->cursor >= map->nb_arg)
			map->cursor -= map->nb_arg;
	if (map->cursor < 0)
			map->cursor += map->nb_arg;
	if (buf == K_SPACE)
	{
		if (map->status[map->cursor])
			map->status[map->cursor] = 0;
		else
			map->status[map->cursor] = 1;
		map->cursor++;
		if (map->cursor >= map->nb_arg)
			map->cursor -= map->nb_arg;
	}
	if (buf == K_REMOVE || buf == K_BACKSPACE)
		remove_arg(map);
	if (buf == K_ESCAPE)
		clean_exit(map);
	if (buf == K_RETURN)
		return_choice(map);
	return (1);
}

void main_loop(t_select *map)
{
	display_all(map);
	while (1)
	{		
		get_controls(map);
		display_all(map);
	}
}

void change_win(int sig)
{
	t_select *map;
	struct winsize size;

	(void)sig;	
	map = get_map(0);
	ioctl(0, TIOCGWINSZ, &size);
	map->nb_co = size.ws_col;
	map->nb_li = size.ws_row;
	display_all(map);
}

void get_signal(void)
{
	signal(SIGWINCH, change_win);
}

int main(int ac, char **av)
{
	t_select *map;
	struct termios old_term;

	signal(SIGWINCH, change_win);
	if (ac == 1)
	{
		ft_putendl_fd("usage: ./ft_select arg1 arg2 ...", 2);
		return (-1);
	}
	if (!term_init(&old_term))
		return (-1);
	if (!init_struct(&map, ac, av, old_term))
		return (-1);
	get_map(map);
	main_loop(map);
	return (0);
}
