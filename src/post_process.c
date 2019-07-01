/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 21:24:39 by mchi              #+#    #+#             */
/*   Updated: 2019/06/26 23:58:31 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	init_pp_framebuffer(t_rt *rt)
{
	glGenTextures(1, &rt->gldata.target_texture);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.target_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RT_WIDTH * 2, RT_HEIGHT * 2,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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
}

static void init_pp_shader(t_rt *rt)
{
	load_shader(rt->buffer, &rt->gldata.pp_fs_id,
		GL_FRAGMENT_SHADER, RT_POST_FS);
	link_program(&rt->gldata.pp_program_id,
		rt->gldata.vs_id, rt->gldata.pp_fs_id);
}

void		pp_uniform_update(t_rt *rt)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.target_texture);
}

void		init_pp(t_rt *rt)
{
	init_pp_framebuffer(rt);
	init_pp_shader(rt);
}

