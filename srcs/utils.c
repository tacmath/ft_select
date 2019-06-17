/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:28:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/17 12:29:59 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

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

int oputchar(int c)
{
	return (write(2, &c, 1));
}
