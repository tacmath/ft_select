/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   event.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:26:29 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/02 15:04:14 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

static void	remove_arg(t_select *map)
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
	map->start = 0;
	center_arg(map, map->cursor);
	display_all(map);
}

static void	return_choice(t_select *map)
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

static void	select_arg(t_select *map)
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
	move_select(map, mem);
}

int			get_controls(t_select *map)
{
	long int	buf;

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
