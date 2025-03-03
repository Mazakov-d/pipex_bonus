/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:03:54 by dmazari           #+#    #+#             */
/*   Updated: 2025/02/26 15:08:38 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	error_pipe(int i)
{
	write(2, "Error : creating pipe\n", 23);
	return (i);
}

int	error_create_cmd(int i)
{
	write(2, "Error : creating command strings\n", 34);
	return (i);
}

int	error_fork(int i, char *path_cmd, int pipe_fd[2])
{
	write(2, "Error : creating child pid\n", 28);
	return (free_close_fd(path_cmd, pipe_fd, i));
}
