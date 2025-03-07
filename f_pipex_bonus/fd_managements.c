/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_managements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:48:27 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:12:03 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	open_switch_stdin(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: Cannot open infile");
		fd = open("/dev/null", O_RDONLY);
		if (fd < 0)
			return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	open_switch_stdout(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error: Cannot open outfile");
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	open_switch_in_out(char *infile, char *outfile)
{
	int infile_status;
	int outfile_status;

	infile_status = open_switch_stdin(infile);
	outfile_status = open_switch_stdout(outfile);
	if (infile_status == 1 && outfile_status == 1)
		return (1);
		
	return (0);
}

void	switch_fd_in_out(int fd_a, int fd_b)
{
	dup2(fd_a, STDIN_FILENO);
	dup2(fd_b, STDOUT_FILENO);
}

int	open_in_out(t_data *data, char **args, int ac)
{
	int		fd;
	char	*infile;

	if (is_here_doc(args[1]))
		infile = ".temp_here_doc.txt";
	else
		infile = args[1];
	fd = open(infile, O_RDONLY);
	if (fd < 0)
		write(2, "Error : Can't read infile\n", 27);
	else
		data->fd_in = fd;
	fd = open(args[ac - 1], O_CREAT || O_TRUNC || O_WRONLY, 0644);
	if (fd < 0)
	{
		write(2, "Error : Can't open outfile\n", 28);
		return (1);
	}
	else
		data->fd_out = fd;
	return (0);
}