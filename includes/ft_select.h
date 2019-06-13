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
# include <termios.h>
# include <termcap.h>


struct					s_select
{
	char **arg;
	char *status;
	int nb_arg;
	int cursor;
	int nb_co;
	int nb_li;
};

typedef struct s_select	t_select;

#endif
