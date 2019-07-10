/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 21:24:39 by mchi              #+#    #+#             */
/*   Updated: 2019/07/10 16:37:44 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static unsigned char	*init_waiting(void)
{
	t_png	png;

	init_png(&png, "resources/loading.png");
	parse_png(&png);
	if (png.width != RT_WIDTH * 2 || png.height != RT_HEIGHT * 2)
		panic("invalid png");
	return (png.data);
}

static void				init_pp_framebuffer(t_rt *rt)
{
	unsigned char	*data;

	data = init_waiting();
	glGenTextures(1, &rt->gldata.target_texture);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.target_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RT_WIDTH * 2, RT_HEIGHT * 2,
		0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &rt->gldata.framebuffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->gldata.framebuffer_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		rt->gldata.target_texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		panic("post processing initializing failed!");
	free(data);
}

static void				init_pp_shader(t_rt *rt)
{
	load_shader(rt->buffer, &rt->gldata.pp_fs_id,
		GL_FRAGMENT_SHADER, RT_POST_FS);
	link_program(&rt->gldata.pp_program_id,
		rt->gldata.vs_id, rt->gldata.pp_fs_id);
}

void					pp_uniform_update(t_rt *rt)
{
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(rt->gldata.render_sampler, 2);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.target_texture);
	glUniform1i(rt->gldata.pp_flags_id, rt->uniforms.pp_flag);
}

void					init_pp(t_rt *rt)
{
	init_pp_framebuffer(rt);
	init_pp_shader(rt);
}
