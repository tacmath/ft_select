/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   event.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:26:29 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/21 12:34:58 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

void get_new_longest_arg(t_select *map)
{
	int n;
	
	map->longest_arg = 0;
	n = -1;
	while (++n < map->nb_arg)
		if ((int)ft_strlen(map->arg[n]) >= map->longest_arg)
			map->longest_arg = ft_strlen(map->arg[n]) + 1;
	if (map->longest_arg + 1 < map->nb_co - 1)
		map->apl = (map->nb_co - 1) / (map->longest_arg + 1);
	else
		map->apl = 0;
	if (map->apl > map->nb_arg)
		map->apl = map->nb_arg;
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
	get_new_longest_arg(map);
	display_all(map);
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
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if ((map->cursor + 1) % map->apl)
		map->cursor++;
	else
		map->cursor -= map->apl - 1;
	tputs(tgetstr("vi", 0), 1, oputchar);
	display_one_arg(map, mem);
	display_one_arg(map, map->cursor);
	move_cursor(map, map->cursor);
	tputs(tgetstr("ve", 0), 1, oputchar);
}

void ft_left(t_select *map)
{
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if (map->cursor % map->apl)
		map->cursor--;
	else
		map->cursor += map->apl - 1;
	tputs(tgetstr("vi", 0), 1, oputchar);
	display_one_arg(map, mem);
	display_one_arg(map, map->cursor);
	move_cursor(map, map->cursor);
	tputs(tgetstr("ve", 0), 1, oputchar);
}

void ft_up(t_select *map)
{
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if (map->cursor - map->apl >= 0)
		map->cursor -= map->apl;
	else
		map->cursor = map->nb_arg - map->nb_arg % map->apl + map->cursor % map->apl;
	if (map->cursor >= map->nb_arg)
		map->cursor -= map->apl;
	tputs(tgetstr("vi", 0), 1, oputchar);
	display_one_arg(map, mem);
	display_one_arg(map, map->cursor);
	move_cursor(map, map->cursor);
	tputs(tgetstr("ve", 0), 1, oputchar);
}

void ft_down(t_select *map)
{
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if (map->cursor + map->apl < map->nb_arg)
		map->cursor += map->apl;
	else
		map->cursor = map->cursor % map->apl;
	tputs(tgetstr("vi", 0), 1, oputchar);
	display_one_arg(map, mem);
	display_one_arg(map, map->cursor);
	move_cursor(map, map->cursor);
	tputs(tgetstr("ve", 0), 1, oputchar);
}

void select_arg(t_select *map)
{
	int mem;

	mem = map->cursor;
	if (map->status[map->cursor])
		map->status[map->cursor] = 0;
	else
		map->status[map->cursor] = 1;
	map->cursor++;
	if (map->cursor >= map->nb_arg)
		map->cursor -= map->nb_arg;
	tputs(tgetstr("vi", 0), 1, oputchar);
	display_one_arg(map, mem);
	display_one_arg(map, map->cursor);
	move_cursor(map, map->cursor);
	tputs(tgetstr("ve", 0), 1, oputchar);
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
		n = map->cursor;
		tputs(tgetstr("vi", 0), 1, oputchar);
		map->cursor = nb;
		display_one_arg(map, n);
		display_one_arg(map, map->cursor);
		move_cursor(map, map->cursor);
		tputs(tgetstr("ve", 0), 1, oputchar);
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
