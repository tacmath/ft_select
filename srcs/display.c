/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   display.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:25:38 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/21 12:38:12 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"
#include <sys/types.h>
#include <sys/stat.h>

void get_color(char *arg)
{
	struct stat buf;

	if (stat(arg, &buf))
		return ;
	if (S_ISREG(buf.st_mode))
		ft_putstr_fd(_CYAN_, 2);
	else if (S_ISDIR(buf.st_mode))
		ft_putstr_fd(_RED_, 2);
	else if (S_ISCHR(buf.st_mode))
		ft_putstr_fd(_YELLOW_, 2);
	else if (S_ISBLK(buf.st_mode))
		ft_putstr_fd(_GREEN_, 2);
	else if (S_ISFIFO(buf.st_mode))
		ft_putstr_fd(_BLUE_, 2);
}

void move_cursor(t_select *map, int nb)
{
	int nb_line;
	int nb_co;

	nb_line = nb / map->apl + 3 - map->start;
	nb_co = (nb % map->apl) * (map->longest_arg + 1) + 1;
	tputs(tgoto(tgetstr("cm", 0), nb_co, nb_line), 1, oputchar);
}

void display_one_arg(t_select *map, int nb)
{
	int len;

	if ((nb / map->apl + 2 - map->start) > (map->nb_li - 3) || nb / map->apl < map->start)
		return ;
	len = ft_strlen(map->arg[nb]);
	move_cursor(map, nb);
	get_color(map->arg[nb]);
	if (map->status[nb])
		ft_putstr_fd(_REVERSE_, 2);
	if (nb == map->cursor)
		ft_putstr_fd(_UNDERLINED_, 2);
	write(2, map->arg[nb], len--);
	ft_putstr_fd(_EOFORMAT_ _EOC_, 2);
}

void draw_start(t_select *map)
{
	char tmp[map->nb_co];
	int n;
	
	n = -1;
	while (++n < map->nb_co)
		tmp[n] = '-';
	tmp[0] = '/';
	tmp[map->nb_co - 1] = '\\';
	write(2, tmp, map->nb_co);
	n = 0;
	while (++n < map->nb_li - 1)
	{
		tputs(tgoto(tgetstr("cm", 0), 0, n), 1, oputchar);
		write(2, "|", 1);
		tputs(tgoto(tgetstr("cm", 0), map->nb_co - 1, n), 1, oputchar);
		write(2, "|", 1);
	}
	tputs(tgoto(tgetstr("cm", 0), 0, n), 1, oputchar);
	tmp[0] = '\\';
	tmp[map->nb_co - 1] = '/';
	write(2, tmp, map->nb_co);
}

int display_all(t_select *map)
{
	int n;

	tputs(tgetstr("cl", 0), 1, oputchar);
	tputs(tgetstr("vi", 0), 1, oputchar);
	draw_start(map);
	if (map->apl == 0 || map->nb_li < 5)
	{
		tputs(tgoto(tgetstr("cm", 0), (map->nb_li / 2), 1), 1, oputchar);
		write(2, "Just Too Small", 15);
		return (1);
	}
	ft_putstr_fd(_UNDERLINED_, 2);
	ft_putstr_fd(_RED_, 2);
	tputs(tgoto(tgetstr("cm", 0), (map->nb_co - ft_strlen("ft_select :")) / 2, 1), 1, oputchar);
	write(2, "ft_select :", 11);
	ft_putstr_fd(_EOFORMAT_ _EOC_, 2);
	n = -1;
	while (map->arg[++n])
		display_one_arg(map, n);
	move_cursor(map, map->cursor);
	tputs(tgetstr("ve", 0), 1, oputchar);
	return (1);
}
