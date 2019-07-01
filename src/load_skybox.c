/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/01 16:23:15 by mchi              #+#    #+#             */
/*   Updated: 2019/06/26 23:50:44 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

/*
**	TODO: make it rigorous.
*/

void	parse_tga(t_tga *tga, t_rt *rt, const char *path)
{
	int fd;

	tga->data = NULL;
	if ((fd = open(path, O_RDONLY)) < 0)
		panic(strerror(errno));
	read(fd, rt->buffer, 2);
	read(fd, &tga->type_code, 1);
	if (tga->type_code != 2)
		panic("skybox error");
	read(fd, rt->buffer, 9);
	read(fd, &tga->width, 2);
	read(fd, &tga->height, 2);
	read(fd, &tga->bit_count, 1);
	read(fd, rt->buffer, 1);
	tga->color_mode = tga->bit_count / 8;
	tga->size = tga->width * tga->height * tga->color_mode;
	tga->data = malloc(sizeof(unsigned char) * tga->size);
	read(fd, tga->data, tga->size);
	close(fd);
}

void	read_tga_texture(t_rt *rt, const char *path, int index)
{
	t_tga	tga;
	int		i;

	parse_tga(&tga, rt, path);
	i = 0;
	while (i < tga.size)
	{
		tga.color_swap = tga.data[i];
		tga.data[i] = tga.data[i + 2];
		tga.data[i + 2] = tga.color_swap;
		i += tga.color_mode;
	}
	if (tga.data)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB,
			tga.width, tga.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tga.data);
	}
	else
		panic("skybox didn't load properly");
	free(tga.data);
}

GLuint		load_skybox(t_rt *rt)
{
	unsigned int		texture_id;
	static const char	*box[] = {"./resources/posx.tga",
		"./resources/negx.tga", "./resources/posy.tga", "./resources/negy.tga",
		"./resources/posz.tga", "./resources/negz.tga"};
	int					i;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	i = -1;
	while (++i < 6)
		read_tga_texture(rt, box[i], i);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return (texture_id);
}
