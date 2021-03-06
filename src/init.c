/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:15:42 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/10 16:44:26 by arherrer         ###   ########.fr       */
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
	gldata->ray_origin_id =
		glGetUniformLocation(gldata->program_id, "ray_origin");
	gldata->rot_id = glGetUniformLocation(gldata->program_id, "rot");
	gldata->aspect_id = glGetUniformLocation(gldata->program_id, "aspect");
	gldata->time_id = glGetUniformLocation(gldata->program_id, "time");
	gldata->mouse_id = glGetUniformLocation(gldata->program_id, "mouse");
	gldata->skybox_id = glGetUniformLocation(gldata->program_id, "skybox");
	gldata->render_sampler =
		glGetUniformLocation(gldata->pp_program_id, "renderedTexture");
	gldata->albedo_sampler =
		glGetUniformLocation(gldata->program_id, "albedoMap");
	gldata->normal_sampler =
		glGetUniformLocation(gldata->program_id, "normalMap");
	gldata->pp_flags_id = glGetUniformLocation(gldata->pp_program_id, "flag");
	if (gldata->ray_origin_id == -1 || gldata->rot_id == -1 ||
		gldata->aspect_id == -1 || gldata->time_id == -1 ||
		gldata->mouse_id == -1 || gldata->skybox_id == -1 ||
		gldata->render_sampler == -1 || gldata->pp_flags_id == -1 ||
		gldata->normal_sampler == -1 || gldata->albedo_sampler == -1)
		panic("shader was corrupt");
}

static void	rt_uniforms_init(t_rt *rt, t_uniforms *uniforms)
{
	int	width;
	int	height;

	glfwGetFramebufferSize(rt->gldata.window, &width, &height);
	rt->uniforms.ray_origin = (t_vec3){0, 1, 8};
	rt->uniforms.rot = (t_vec2){0, 0};
	rt->uniforms.aspect = (float)width / height;
	uniforms->time.y = glfwGetTime();
	uniforms->time.x = glfwGetTime();
	uniforms->skybox = load_skybox(rt);
	uniforms->pp_flag = 0;
	load_textures(rt);
	glUseProgram(rt->gldata.program_id);
	glUniform1i(rt->gldata.skybox_id, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, uniforms->skybox);
	glUniform1i(rt->gldata.albedo_sampler, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.albedo_map);
	glUniform1i(rt->gldata.normal_sampler, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, rt->gldata.normal_map);
}

t_rt		*init(t_rt *rt, const char *path)
{
	rt_gldata_init(rt->buffer, rt, path);
	init_callbacks(rt);
	init_pp(rt);
	rt_getuniforms(&rt->gldata);
	rt_uniforms_init(rt, &rt->uniforms);
	return (rt);
}

void		panic(const char *msg)
{
	ft_fdputstr("error: ", 2);
	ft_fdputstr(msg, 2);
	ft_fdputstr("\n", 2);
	exit(EXIT_FAILURE);
}
