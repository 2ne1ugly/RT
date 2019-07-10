/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 15:13:50 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/09 23:02:29 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../include/rt.h"

// TODO: use libft version of funcitons
void call_loop(const char *scene)
{
	static const char	*ex = "./RT ";
	char *const str = malloc(strlen(ex) + strlen(scene) + 1);
	if (str)
	{
		strcpy(str, ex);
		strcat(str, scene);
		system(str);
	}
	free(str);
}

int	main(int argc, char **argv)
{
	static struct s_rt	rt;
	int					cnt;

	if (argc < 2)
	{
		return (ft_fdputstr("usage: ./RTv1 <scene>\n", 2));
	}
	if (argc == 2)
	{
		loop(init(&rt, argv[1]));
		return 0;
	}
	cnt = 1;
	while (cnt < argc)
		call_loop(argv[cnt++]);
}
