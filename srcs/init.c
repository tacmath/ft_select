/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   init.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:27:09 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/02 14:35:38 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

int	init_struct(t_select **map, int ac, char **av, struct termios old)
{
	int n;

	if (!(*map = ft_memalloc(sizeof(t_select))))
		return (0);
	if (!((*map)->arg = ft_memalloc(sizeof(char*) * ac)))
		return (0);
	if (!((*map)->status = ft_memalloc(sizeof(char) * (ac - 1))))
		return (0);
	n = 0;
	while (++n < ac)
	{
		(*map)->arg[n - 1] = ft_strdup(av[n]);
		if ((int)ft_strlen(av[n]) > (*map)->longest_arg)
			(*map)->longest_arg = ft_strlen(av[n]);
	}
	(*map)->old_term = old;
	(*map)->nb_arg = ac - 1;
	(*map)->nb_co = tgetnum("co");
	(*map)->nb_li = tgetnum("li");
	if ((*map)->longest_arg + 1 < (*map)->nb_co - 1)
		(*map)->apl = ((*map)->nb_co - 1) / ((*map)->longest_arg + 1);
	if ((*map)->apl > (*map)->nb_arg)
		(*map)->apl = (*map)->nb_arg;
	return (1);
}

int	term_init(struct termios *old)
{
	struct termios config;

	if (!isatty(0))
		return (0);
	if (tcgetattr(0, &config) < 0 || tcgetattr(0, old) < 0)
		return (0);
	config.c_lflag &= ~(ECHO | ICANON);
	config.c_cc[VMIN] = 1;
	config.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &config) < 0)
		return (0);
	if (tgetent(0, getenv("TERM")) < 1)
		return (0);
	tputs(tgetstr("ti", 0), 1, oputchar);
	return (1);
}
