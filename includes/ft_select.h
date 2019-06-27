/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_select.h                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/11 11:01:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/20 17:08:18 by mtaquet     ###    #+. /#+    ###.fr     */
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

#define K_UP 4283163
#define K_DOWN 4348699
#define K_RIGHT 4414235
#define K_LEFT 4479771
#define K_SPACE 32
#define K_RETURN 10
#define K_REMOVE 2117294875
#define K_BACKSPACE 127
#define K_ESCAPE 27

struct					s_select
{
	char **arg;
	char *status;
	struct termios old_term;
	int nb_arg;
	int cursor;
	int start;
	int longest_arg;
	int apl;
	int nb_co;
	int nb_li;
};

typedef struct s_select	t_select;

void free_struct(t_select *map);
void clean_exit(t_select *map);
t_select *get_map(t_select *map);
int oputchar(int c);
void move_cursor(t_select *map, int nb);
void display_one_arg(t_select *map, int nb);
int display_all(t_select *map);
void get_signal(void);
int center_arg(t_select *map, int arg);
int init_struct(t_select **map, int ac, char **av, struct termios old);
int term_init(struct termios *old);
int get_controls(t_select *map);

#endif
