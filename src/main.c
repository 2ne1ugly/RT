/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 15:13:50 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/11 12:25:09 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../include/rt.h"

int		main(int argc, char **argv)
{
	static struct s_rt	rt;

	if (argc != 2)
	{
		return (ft_fdputstr("usage: ./RTv1 <scene>\n", 2));
	}
	loop(init(&rt, argv[1]));
}
