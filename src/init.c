/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:15:42 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/25 21:14:25 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	rt_gldata_init(char *buffer, t_rt *rt, const char *path)
{
	make_window(&rt->gldata);
	load_scene(rt, path);
	load_glsl(buffer, &rt->gldata);
}

static void	rt_getuniforms(t_gldata *gldata)
{
	gldata->ray_origin_id = glGetUniformLocation(gldata->program_id,
		"ray_origin");
	if (gldata->ray_origin_id == -1)
		panic("shader was corrupt");
	gldata->rot_id = glGetUniformLocation(gldata->program_id, "rot");
	if (gldata->rot_id == -1)
		panic("shader was corrupt");
	gldata->resolution_id = glGetUniformLocation(gldata->program_id,
		"resolution");
	if (gldata->resolution_id == -1)
		panic("shader was corrupt");
	gldata->time_id = glGetUniformLocation(gldata->program_id, "time");
	if (gldata->time_id == -1)
		panic("shader was corrupt");
	gldata->mouse_id = glGetUniformLocation(gldata->program_id, "mouse");
	if (gldata->mouse_id == -1)
		panic("shader was corrupt");
}

static void	rt_uniforms_init(t_rt *rt, t_uniforms *uniforms)
{
	int	width;
	int	height;

	glfwGetFramebufferSize(rt->gldata.window, &width, &height);
	rt->uniforms.ray_origin = (t_vec3){0, 1, 8};
	rt->uniforms.rot = (t_vec2){0, 0};
	rt->uniforms.resolution = (t_vec2){width, height};
	uniforms->time.y = glfwGetTime();
	uniforms->time.x = glfwGetTime();
}

t_rt		*init(t_rt *rt, const char *path)
{
	rt_gldata_init(rt->buffer, rt, path);
	rt_uniforms_init(rt, &rt->uniforms);
	init_callbacks(rt);
	rt_getuniforms(&rt->gldata);
	return (rt);
}

void		panic(const char *msg)
{
	ft_fdputstr("error: ", 2);
	ft_fdputstr(msg, 2);
	ft_fdputstr("\n", 2);
	exit(EXIT_FAILURE);
}
