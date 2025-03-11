/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_management.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:48:27 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/11 15:56:05 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_pipes(t_data *data)
{
	int	i;
	int	pipe_count;

	pipe_count = data->cmd_count - 1;
	data->fd.pipe_count = pipe_count;
	if (pipe_count <= 0)
	{
		data->fd.pipes = NULL;
		return (0);
	}
	data->fd.pipes = malloc(sizeof(int *) * pipe_count);
	if (!data->fd.pipes)
		return (error_return("Error: Malloc failed for pipes", 1));
	i = 0;
	while (i < pipe_count)
	{
		data->fd.pipes[i] = malloc(sizeof(int) * 2);
		if (!data->fd.pipes[i])
			return (clean_exit(data, 1, "Error: Malloc failed for pipe"));
		if (pipe(data->fd.pipes[i]) == -1)
			return (clean_exit(data, 1, "Error: Pipe creation failed"));
		i++;
	}
	return (0);
}

int	open_input_file(t_data *data, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: Cannot open input file");
		fd = open("/dev/null", O_RDONLY);
		if (fd < 0)
			return (1);
	}
	data->fd.in_file = fd;
	return (0);
}

int	open_output_file(t_data *data, char *filename)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror("Error: Cannot open output file");
		return (1);
	}
	data->fd.out_file = fd;
	return (0);
}

int	setup_files(t_data *data, char **args, int ac)
{
	char	*infile;

	if (data->here_doc)
		infile = ".temp_here_doc.txt";
	else
		infile = args[1];
	if (open_input_file(data, infile) != 0)
	{
		print_error("Error: Failed to open input file\n");
		return (1);
	}
	if (open_output_file(data, args[ac - 1]) != 0)
	{
		print_error("Error: Failed to open output file\n");
		return (clean_exit(data, 1, NULL));
	}
	return (0);
}

void	redirect_io(int fd_in, int fd_out)
{
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("Error: Failed to redirect input");
		exit(1);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("Error: Failed to redirect output");
		exit(1);
	}
}
