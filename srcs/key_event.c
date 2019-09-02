/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   key_event.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/02 15:03:04 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/02 16:09:39 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

void		ft_right(t_select *map)
{
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if ((map->cursor + 1) % map->apl && (map->cursor + 1) != map->nb_arg)
		map->cursor++;
	else
		map->cursor = (map->cursor / map->apl) * map->apl;
	move_select(map, mem);
}

void		ft_left(t_select *map)
{
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if (map->cursor % map->apl)
		map->cursor--;
	else
	{
		map->cursor += map->apl - 1;
		if (map->cursor > map->nb_arg - 1)
			map->cursor = map->nb_arg - 1;
	}
	move_select(map, mem);
}

static void	ft_up_limit(t_select *map, int mem)
{
	if (mem / map->apl == map->start && map->start)
	{
		map->start--;
		display_all(map);
		return ;
	}
	if (mem / map->apl == map->start && (map->nb_arg - 1)
		/ map->apl >= map->nb_li - 4)
	{
		map->start = (map->nb_arg - 1) / map->apl - (map->nb_li - 5);
		if (map->start < 0)
			map->start = 0;
		display_all(map);
		return ;
	}
	move_select(map, mem);
}

void		ft_up(t_select *map)
{
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if (map->cursor - map->apl >= 0)
		map->cursor -= map->apl;
	else
		map->cursor = map->nb_arg - map->nb_arg % map->apl
			+ map->cursor % map->apl;
	if (map->cursor >= map->nb_arg)
		map->cursor -= map->apl;
	ft_up_limit(map, mem);
}

void		ft_down(t_select *map)
{
	int mem;

	if (!map->apl)
		return ;
	mem = map->cursor;
	if (map->cursor + map->apl < map->nb_arg)
		map->cursor += map->apl;
	else
		map->cursor = map->cursor % map->apl;
	if (map->cursor / map->apl == 0 && (map->nb_arg - 1)
		/ map->apl >= map->nb_li - 4)
	{
		map->start = 0;
		display_all(map);
		return ;
	}
	if (mem / map->apl + 1 - map->start == map->nb_li - 4
		&& (map->nb_arg - 1) / map->apl - map->start >= map->nb_li - 4)
	{
		map->start++;
		display_all(map);
		return ;
	}
	move_select(map, mem);
}
