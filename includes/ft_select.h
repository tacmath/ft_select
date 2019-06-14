/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_select.h                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/11 11:01:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 12:46:00 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <unistd.h>
# include "libft.h"
# include <stdlib.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <termcap.h>

# define _UNDERLINED_	"\e[4m"
# define _REVERSE_	"\e[7m"
# define _EOFORMAT_	"\e[0m"

# define _BLACK_	"\e[30m"
# define _RED_		"\e[31m"
# define _GREEN_	"\e[32m"
# define _YELLOW_	"\e[33m"
# define _BLUE_		"\e[34m"
# define _MAGENTA_	"\e[35m"
# define _CYAN_		"\e[36m"
# define _WGITE_	"\e[37m"
# define _EOC_		"\e[39m"



struct					s_select
{
	char **arg;
	char *status;
	struct termios old_term;
	int nb_arg;
	int cursor;
	int longest_arg;
	int nb_co;
	int nb_li;
};

typedef struct s_select	t_select;

#endif
