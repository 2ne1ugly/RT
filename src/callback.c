/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   callback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:43:41 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/07 23:49:11 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	rt_char_cb_(GLFWwindow *window, t_uniforms *uniforms)
{
	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS))
		uniforms->ray_origin.z -= 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS))
		uniforms->ray_origin.z += 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS))
		uniforms->ray_origin.x -= 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))
		uniforms->ray_origin.x += 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS))
		uniforms->ray_origin.y += 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS))
		uniforms->ray_origin.y -= 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS))
		uniforms->rot.x -= 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS))
		uniforms->rot.x += 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS))
		uniforms->rot.y -= 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
		uniforms->rot.y += 4.2 * uniforms->delta_time;
	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS))
	{
		uniforms->ray_origin = (t_vec3){0, 1, 8};
		uniforms->rot = (t_vec2){0, 0};
	}
}

static void	rt_char_cb(GLFWwindow *window, unsigned int codepoint)
{
	(void)codepoint;
	rt_char_cb_(window, &((t_rt*)glfwGetWindowUserPointer(window))->uniforms);
}

static void	rt_cursor_pos_cb(GLFWwindow *window, double xpos, double ypos)
{
	t_rt *const	rt = glfwGetWindowUserPointer(window);

	rt->uniforms.mouse.z = rt->uniforms.mouse.x;
	rt->uniforms.mouse.w = rt->uniforms.mouse.y;
	rt->uniforms.mouse.x = xpos;
	rt->uniforms.mouse.y = ypos;
}

static void	rt_cursor_enter_cb(GLFWwindow *window, int entered)
{
	t_rt *const	rt = glfwGetWindowUserPointer(window);

	rt->uniforms.entered = entered;
}

void		init_callbacks(t_rt *rt)
{
	glfwSetWindowUserPointer(rt->gldata.window, rt);
	glfwSetCharCallback(rt->gldata.window, rt_char_cb);
	glfwSetCursorPosCallback(rt->gldata.window, rt_cursor_pos_cb);
	glfwSetCursorEnterCallback(rt->gldata.window, rt_cursor_enter_cb);
}
