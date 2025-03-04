/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:03:54 by dmazari           #+#    #+#             */
/*   Updated: 2025/03/03 20:34:38 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	error_pipe(int i)
{
	write(2, "Error : creating pipe\n", 23);
	return (i);
}

int	error_create_cmd(int pipe_fd[2], int i)
{
	if (pipe_fd)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	write(2, "Error : creating command strings\n", 34);
	return (i);
}

int	error_fork(int i, char *path_cmd, int pipe_fd[2])
{
	write(2, "Error : creating child pid\n", 28);
	return (free_close_fd(path_cmd, pipe_fd, i));
}
