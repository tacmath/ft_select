/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   signal.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/17 12:28:29 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/02 14:33:40 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_select.h"

static void	change_win(int sig)
{
	t_select		*map;
	struct winsize	size;

	(void)sig;
	map = get_map(0);
	ioctl(0, TIOCGWINSZ, &size);
	map->nb_co = size.ws_col;
	map->nb_li = size.ws_row;
	if (map->longest_arg + 1 < map->nb_co - 1)
		map->apl = (map->nb_co - 1) / (map->longest_arg + 1);
	else
		map->apl = 0;
	if (map->apl > map->nb_arg)
		map->apl = map->nb_arg;
	map->start = 0;
	center_arg(map, map->cursor);
	display_all(map);
}

static void	sig_quit(int sig)
{
	(void)sig;
	clean_exit(get_map(0));
}

static void	suspend_term(int sig)
{
	t_select	*map;
	char		tmp[2];

	(void)sig;
	map = get_map(0);
	tmp[0] = map->old_term.c_cc[VSUSP];
	tmp[1] = 0;
	tcsetattr(0, TCSANOW, &map->old_term);
	signal(SIGTSTP, SIG_DFL);
	tputs(tgetstr("te", 0), 1, oputchar);
	ioctl(0, TIOCSTI, tmp);
}

static void	restart_term(int sig)
{
	struct termios config;

	(void)sig;
	tcgetattr(0, &config);
	config.c_lflag &= ~(ECHO | ICANON);
	config.c_cc[VMIN] = 1;
	config.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &config);
	tputs(tgetstr("ti", 0), 1, oputchar);
	display_all(get_map(0));
}

void		get_signal(void)
{
	signal(SIGWINCH, change_win);
	signal(SIGINT, sig_quit);
	signal(SIGQUIT, sig_quit);
	signal(SIGTERM, sig_quit);
	signal(SIGTSTP, suspend_term);
	signal(SIGCONT, restart_term);
}
