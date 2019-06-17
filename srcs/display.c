/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   display.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:25:38 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/17 12:39:24 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

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
	move_cursor(map, nb);
	if (map->status[nb])
		ft_putstr(_REVERSE_);
	if (nb == map->cursor)
		ft_putstr(_UNDERLINED_);
	write(0, map->arg[nb], len--);
	ft_putstr(_EOFORMAT_ _EOC_);
}

int display_all(t_select *map)
{
	int n;
	int m;
	int ret;

	tputs(tgetstr("cl", 0), 1, oputchar);
	tputs(tgetstr("vi", 0), 1, oputchar);
	write(0, "ft_select :", 11);
	n = -1;
	m = 0;
	ret = 0;
	while (map->arg[++n])
	{
		display_one_arg(map, n);
		if (n == map->cursor)
			ret = n;
	}
	move_cursor(map, ret);
	tputs(tgetstr("ve", 0), 1, oputchar);
	return (1);
}
