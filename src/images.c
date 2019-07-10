/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfaria <zfaria@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 17:40:06 by zfaria            #+#    #+#             */
/*   Updated: 2019/07/09 19:24:49 by zfaria           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

#define WID RT_WIDTH * 2
#define HEI RT_HEIGHT * 2
#define MAX_PALETTE_LEN PNG_MAX_PALETTE_LENGTH

uint8_t		*get_raw_pixels(t_rt *rt)
{
	uint8_t	*pixels;

	pixels = malloc(WID * HEI * 4);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.target_texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WID, HEI);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, WID, HEI, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
	return (pixels);
}

void		destroy_struct(t_image *img)
{
	png_free(img->png, img->palette);
	png_destroy_write_struct(&img->png, &img->info);
	fclose(img->fp);
	free(img->rows);
	free(img->pixels);
}

void		save_image(t_rt *rt)
{
	t_image	img;
	int		i;
	char	buf[80];
	time_t	t;

	img.pixels = get_raw_pixels(rt);
	img.png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	img.info = png_create_info_struct(img.png);
	t = time(0);
	strftime(buf, sizeof(buf), "rt-%H%M%S%d%m%y.png", localtime(&t));
	img.fp = fopen(buf, "wb");
	png_init_io(img.png, img.fp);
	png_set_IHDR(img.png, img.info, WID, HEI, 8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	img.palette = png_malloc(img.png, sizeof(png_color) * MAX_PALETTE_LEN);
	png_set_PLTE(img.png, img.info, img.palette, MAX_PALETTE_LEN);
	png_write_info(img.png, img.info);
	png_set_packing(img.png);
	img.rows = png_malloc(img.png, HEI * sizeof(png_bytep));
	i = -1;
	while (++i < HEI)
		img.rows[i] = (png_bytep)(img.pixels + (HEI - i) * WID * 3);
	png_write_image(img.png, img.rows);
	png_write_end(img.png, img.info);
	destroy_struct(&img);
}
