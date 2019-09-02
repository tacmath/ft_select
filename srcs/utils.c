/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:28:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/02 14:34:34 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

void		free_struct(t_select *map)
{
	int n;

	n = -1;
	while (map->arg[++n])
		free(map->arg[n]);
	free(map->arg);
	free(map->status);
	free(map);
}

void		clean_exit(t_select *map)
{
	tputs(tgetstr("te", 0), 1, oputchar);
	tcsetattr(0, TCSANOW, &map->old_term);
	free_struct(map);
	exit(0);
}

t_select	*get_map(t_select *map)
{
	static t_select *tmp = 0;

	if (!tmp)
		tmp = map;
	return (tmp);
}

int			oputchar(int c)
{
	return (write(2, &c, 1));
}

int			center_arg(t_select *map, int arg)
{
	if (!map->apl)
		return (1);
	if (arg / map->apl - map->start < map->nb_li - 4 && arg
		/ map->apl >= map->start)
		return (0);
	if (arg / map->apl < map->nb_li - 4)
		map->start = 0;
	else if (arg / map->apl >= (map->nb_arg - 1) / map->apl - (map->nb_li - 4))
		map->start = (map->nb_arg - 1) / map->apl - (map->nb_li - 5);
	else
		map->start = arg / map->apl - ((map->nb_li - 4) / 2);
	return (1);
}
