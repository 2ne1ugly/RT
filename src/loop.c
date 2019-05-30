/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:13:10 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/24 21:20:13 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	update(t_rt *rt)
{
	glUniform3f(rt->gldata.ray_origin_id, rt->uniforms.ray_origin.x,
		rt->uniforms.ray_origin.y, rt->uniforms.ray_origin.z);
	glUniform2f(rt->gldata.resolution_id, rt->uniforms.resolution.x,
		rt->uniforms.resolution.y);
	rt->uniforms.time.y = rt->uniforms.time.x;
	rt->uniforms.time.x = glfwGetTime();
	rt->uniforms.delta_time = rt->uniforms.time.x - rt->uniforms.time.y;
	glUniform2f(rt->gldata.time_id, rt->uniforms.time.x, rt->uniforms.time.y);
	glUniform4f(rt->gldata.mouse_id, rt->uniforms.mouse.x, rt->uniforms.mouse.y,
		rt->uniforms.mouse.z, rt->uniforms.mouse.w);
	glUniform2f(rt->gldata.rot_id, rt->uniforms.rot.x, rt->uniforms.rot.y);
}

static void	render(const t_gldata *const gldata)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, gldata->vertex_buffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glfwSwapBuffers(gldata->window);
}

void		loop(t_rt *rt)
{
	while ((glfwGetKey(rt->gldata.window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
		&& (!glfwWindowShouldClose(rt->gldata.window)))
	{
		glfwPollEvents();
		if (rt->uniforms.entered)
		{
			update(rt);
			render(&rt->gldata);
		}
	}
}
