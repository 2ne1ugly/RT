/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:13:10 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/10 16:30:33 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	uniform_update(t_rt *rt)
{
	glUniform3f(rt->gldata.ray_origin_id, rt->uniforms.ray_origin.x,
		rt->uniforms.ray_origin.y, rt->uniforms.ray_origin.z);
	glUniform1f(rt->gldata.aspect_id, rt->uniforms.aspect);
	rt->uniforms.time.y = rt->uniforms.time.x;
	rt->uniforms.time.x = glfwGetTime();
	rt->uniforms.delta_time = rt->uniforms.time.x - rt->uniforms.time.y;
	glUniform2f(rt->gldata.time_id, rt->uniforms.time.x, rt->uniforms.time.y);
	glUniform4f(rt->gldata.mouse_id, rt->uniforms.mouse.x, rt->uniforms.mouse.y,
		rt->uniforms.mouse.z, rt->uniforms.mouse.w);
	glUniform2f(rt->gldata.rot_id, rt->uniforms.rot.x, rt->uniforms.rot.y);
}

static void	render(t_rt *rt)
{
	glUseProgram(rt->gldata.program_id);
	uniform_update(rt);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->gldata.framebuffer_id);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, rt->gldata.vertex_buffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

void		post_render(t_rt *rt)
{
	glUseProgram(rt->gldata.pp_program_id);
	pp_uniform_update(rt);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, rt->gldata.vertex_buffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

void		loop(t_rt *rt)
{
	post_render(rt);
	glfwSwapBuffers(rt->gldata.window);
	while ((glfwGetKey(rt->gldata.window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
		&& (!glfwWindowShouldClose(rt->gldata.window)))
	{
		glfwPollEvents();
		if (rt->uniforms.entered)
		{
			render(rt);
			post_render(rt);
			glfwSwapBuffers(rt->gldata.window);
			if (rt->uniforms.save_screenshot)
			{
				save_image(rt);
				rt->uniforms.save_screenshot = 0;
			}
		}
	}
}
