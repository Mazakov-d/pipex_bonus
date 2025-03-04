/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:03:54 by dmazari           #+#    #+#             */
/*   Updated: 2025/03/04 14:49:24 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	error_pipe(int i)
{
	write(2, "Error: Creating pipe\n", 21);
	return (i);
}

int	error_create_cmd(int pipe_fd[2], int i)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	write(2, "Error: Creating command strings\n", 32);
	return (i);
}

int	error_fork(int i, char *path_cmd, int pipe_fd[2])
{
	write(2, "Error: Creating child process\n", 30);
	return (free_close_fd(path_cmd, pipe_fd, i));
}