/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   event.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:26:29 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/17 15:22:50 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

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

void ft_right(t_select *map)
{
	int apl;

	apl = map->nb_co / (map->longest_arg + 1);
	if (apl > map->nb_arg)
		apl = map->nb_arg;
	if ((map->cursor + 1) % apl)
		map->cursor++;
	else
		map->cursor -= --apl;
}

void ft_left(t_select *map)
{
	int apl;

	apl = map->nb_co / (map->longest_arg + 1);
	if (apl > map->nb_arg)
		apl = map->nb_arg;
	if (map->cursor % apl)
		map->cursor--;
	else
		map->cursor += --apl;
}

void ft_up(t_select *map)
{
	int apl;

	apl = map->nb_co / (map->longest_arg + 1);
	if (map->cursor - apl >= 0)
		map->cursor -= apl;
	else
		map->cursor = map->nb_arg - map->nb_arg % apl + map->cursor % apl;
	if (map->cursor >= map->nb_arg)
		map->cursor -= apl;
}

void ft_down(t_select *map)
{
	int apl;

	apl = map->nb_co / (map->longest_arg + 1);
	if (map->cursor + apl < map->nb_arg)
		map->cursor += apl;
	else
		map->cursor = map->cursor % apl;
}

void select_arg(t_select *map)
{
	if (map->status[map->cursor])
		map->status[map->cursor] = 0;
	else
		map->status[map->cursor] = 1;
	map->cursor++;
	if (map->cursor >= map->nb_arg)
		map->cursor -= map->nb_arg;
}

int cmp_all(t_select *map, char *line)
{

	int		ret;
	int		nb;
	int		n;

	ret = 0;
	nb = 0;
	n = -1;
	while (++n < map->nb_arg)
		if (ft_strstr(map->arg[n], line))
		{
			ret++;
			nb = n;
		}
	if (!ret && (n = -1))
		while (line[++n])
			line[n] = line[n + 1];
	else if (ret == 1)
	{
		map->cursor = nb;
		return (1);
	}
	else
		return (1);
	return (0);
}

int search_arg(t_select *map, char *new)
{
	static char *line = 0;
	char	*tmp;

	if ((!line && !(line = ft_memalloc(1)))
			|| !(tmp = ft_strjoin(line, new)))
		return (0);
	free(line);
	line = tmp;
	while (line[0])
		if (cmp_all(map, line))
			return (1);
	return (1);
}

int get_controls(t_select *map)
{
	long int    buf;

	buf = 0;
	read(0, &buf, 7);
	if (buf == K_UP)
		ft_up(map);
	else if (buf == K_DOWN)
		ft_down(map);
	else if (buf == K_RIGHT)
		ft_right(map);
	else if (buf == K_LEFT)
		ft_left(map);
	else if (buf == K_SPACE)
		select_arg(map);
	else if (buf == K_REMOVE || buf == K_BACKSPACE)
		remove_arg(map);
	else if (buf == K_ESCAPE)
		clean_exit(map);
	else if (buf == K_RETURN)
		return_choice(map);
	else if (!search_arg(map, (char*)(&buf)))
		return (0);
	return (1);
}
