/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   signal.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:28:29 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/17 12:34:12 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

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
