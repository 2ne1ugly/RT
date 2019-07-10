/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/01 16:23:15 by mchi              #+#    #+#             */
/*   Updated: 2019/07/10 16:37:20 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

void	parse_png(t_png *png)
{
	unsigned int i;

	png->row_bytes = png_get_rowbytes(png->png_ptr, png->info_ptr);
	png->data = (unsigned char*)malloc(png->row_bytes * png->height);
	png->rows = png_get_rows(png->png_ptr, png->info_ptr);
	i = 0;
	while (i < png->height)
	{
		ft_memcpy(png->data + (png->row_bytes * i),
			png->rows[i], png->row_bytes);
		i++;
	}
	png_destroy_read_struct(&png->png_ptr, &png->info_ptr, NULL);
	fclose(png->fp);
}

void	init_png(t_png *png, const char *path)
{
	if ((png->fp = fopen(path, "rb")) == NULL)
		panic("cannot open skybox");
	png->png_ptr = png_create_read_struct(
		PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png->png_ptr == NULL)
		panic("cannot open skybox");
	png->info_ptr = png_create_info_struct(png->png_ptr);
	if (png->info_ptr == NULL)
		panic("cannot open skybox");
	if (setjmp(png_jmpbuf(png->png_ptr)))
		panic("cannot open skybox");
	png_init_io(png->png_ptr, png->fp);
	png_set_sig_bytes(png->png_ptr, 0);
	png_read_png(png->png_ptr, png->info_ptr, PNG_TRANSFORM_STRIP_16 |
		PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
	png_get_IHDR(png->png_ptr, png->info_ptr, &png->width, &png->height,
		&png->bit_depth, &png->color_type, &png->interlace_type, NULL, NULL);
}

void	read_png_texture(t_rt *rt, const char *path, int index)
{
	t_png	png;

	(void)rt;
	init_png(&png, path);
	parse_png(&png);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB,
		png.width, png.height, 0, GL_RGB, GL_UNSIGNED_BYTE, png.data);
	free(png.data);
}

GLuint	load_skybox(t_rt *rt)
{
	unsigned int		texture_id;
	static const char	*box[] = {"./resources/posx.png",
		"./resources/negx.png", "./resources/posy.png", "./resources/negy.png",
		"./resources/posz.png", "./resources/negz.png"};
	int					i;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	i = -1;
	while (++i < 6)
		read_png_texture(rt, box[i], i);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return (texture_id);
}
