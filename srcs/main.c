/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:28:19 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/17 12:34:40 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

void main_loop(t_select *map)
{
	display_all(map);
	while (1)
	{		
		get_controls(map);
		display_all(map);
	}
}

int main(int ac, char **av)
{
	t_select *map;
	struct termios old_term;

	get_signal();
	if (ac == 1)
	{
		ft_putendl_fd("usage: ./ft_select arg1 arg2 ...", 2);
		return (-1);
	}
	if (!term_init(&old_term))
		return (-1);
	if (!init_struct(&map, ac, av, old_term))
		return (-1);
	get_map(map);
	main_loop(map);
	return (0);
}
