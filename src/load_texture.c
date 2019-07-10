/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 02:53:43 by mchi              #+#    #+#             */
/*   Updated: 2019/07/10 03:49:06 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

void load_albedo(t_rt *rt)
{
	t_png png;

	glGenTextures(1, &rt->gldata.albedo_map);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.albedo_map);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	init_png(&png, "resources/albedo.png");
	parse_png(&png);
	glTexImage2D(GL_TEXTURE_2D , 0, GL_RGBA,
		png.width, png.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, png.data);
	free(png.data);

	// init_png(&png, "resources/albedo.png");
	// parse_png(&png);
	// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB,
	// 	png.width, png.height, 0, GL_RGB, GL_UNSIGNED_BYTE, png.data);
	// free(png.data);

}
