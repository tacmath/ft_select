/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils2.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/02 15:01:14 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/02 15:29:09 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

void		get_new_longest_arg(t_select *map)
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

void		move_select(t_select *map, int last_pos)
{
	tputs(tgetstr("vi", 0), 1, oputchar);
	display_one_arg(map, last_pos);
	display_one_arg(map, map->cursor);
	move_cursor(map, map->cursor);
	tputs(tgetstr("ve", 0), 1, oputchar);
}

static int	check_ret(t_select *map, int ret, int nb)
{
	int mem;

	if (ret == 1)
	{
		mem = map->cursor;
		map->cursor = nb;
		if (center_arg(map, map->cursor))
		{
			display_all(map);
			return (1);
		}
		move_select(map, mem);
		return (1);
	}
	else
		return (1);
	return (0);
}

static int	cmp_all(t_select *map, char *line)
{
	int	ret;
	int	nb;
	int	n;

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
	else if (check_ret(map, ret, nb))
		return (1);
	return (0);
}

int			search_arg(t_select *map, char *new)
{
	static char	*line = 0;
	char		*tmp;

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
