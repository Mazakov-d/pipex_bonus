/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:25:03 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/18 18:10:56 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	open_switch_stdin(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	open_switch_stdout(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	open_switch_in_out(char *infile, char *outfile)
{
	if (open_switch_stdin(infile) == 1)
		return (1);
	if (open_switch_stdout(outfile) == 1)
		return (1);
	return (0);
}

void	switch_fd_in_out(int fd_a, int fd_b)
{
	dup2(fd_a, STDIN_FILENO);
	dup2(fd_b, STDOUT_FILENO);
}