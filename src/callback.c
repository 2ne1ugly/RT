/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   callback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:43:41 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/09 21:13:22 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"


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

void dispatch_key_press1(t_uniforms *u, int key)
{
	if (key == GLFW_KEY_W)
		u->ray_origin.z -= 4.2 * u->delta_time;
	if (key == GLFW_KEY_S)
		u->ray_origin.z += 4.2 * u->delta_time;
	if (key == GLFW_KEY_A)
		u->ray_origin.x -= 4.2 * u->delta_time;
	if (key == GLFW_KEY_D)
		u->ray_origin.x += 4.2 * u->delta_time;
	if (key == GLFW_KEY_J)
		u->ray_origin.y += 4.2 * u->delta_time;
	if (key == GLFW_KEY_K)
		u->ray_origin.y -= 4.2 * u->delta_time;
	if (key == GLFW_KEY_Q)
		u->rot.x -= 4.2 * u->delta_time;
	if (key == GLFW_KEY_E)
		u->rot.x += 4.2 * u->delta_time;
	if (key == GLFW_KEY_H)
		u->rot.y -= 4.2 * u->delta_time;
	if (key == GLFW_KEY_L)
		u->rot.y += 4.2 * u->delta_time;
	if (key == GLFW_KEY_SPACE)
	{
		u->ray_origin = (t_vec3){0, 1, 8};
		u->rot = (t_vec2){0, 0};
	}

}

void dispatch_key_press2(t_uniforms *u, int key)
{
	if (key == GLFW_KEY_1)
		u->pp_flag ^= 1;
	if (key == GLFW_KEY_2)
		u->pp_flag ^= 2;
	if (key == GLFW_KEY_3)
		u->pp_flag ^= 4;
	if (key == GLFW_KEY_P)
		u->save_screenshot = 1;
}

static void rt_key_cb(GLFWwindow* window, int key, int scancode, int action)
{
	t_rt *const	rt = glfwGetWindowUserPointer(window);
	(void)scancode;

	if (action == GLFW_PRESS)
	{
		dispatch_key_press1(&rt->uniforms, key);
		dispatch_key_press2(&rt->uniforms, key);
	}
	if (action == GLFW_REPEAT)
		dispatch_key_press1(&rt->uniforms, key);
}

void		init_callbacks(t_rt *rt)
{
	glfwSetWindowUserPointer(rt->gldata.window, rt);
	glfwSetKeyCallback(rt->gldata.window,
		(void (*)(struct GLFWwindow *, int, int, int, int))rt_key_cb);
	glfwSetCursorPosCallback(rt->gldata.window, rt_cursor_pos_cb);
	glfwSetCursorEnterCallback(rt->gldata.window, rt_cursor_enter_cb);
}
