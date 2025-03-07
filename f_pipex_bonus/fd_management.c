/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_management.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:48:27 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:23:49 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Initialize the pipes for the commands
 * 
 * @param data Main data structure
 * @param ac Argument count
 * @param av Argument values
 * @return int 0 on success, 1 on failure
 */
int	init_pipes(t_data *data, int ac, char **av)
{
	int	i;
	int	pipe_count;

	(void)ac;
	(void)av;
	// Calculate number of pipes needed (number of commands - 1)
	pipe_count = data->cmd_count - 1;
	data->fd.pipe_count = pipe_count;
	
	// No pipes needed if only one command
	if (pipe_count <= 0)
	{
		data->fd.pipes = NULL;
		return (0);
	}
	
	// Allocate memory for pipe array
	data->fd.pipes = malloc(sizeof(int *) * pipe_count);
	if (!data->fd.pipes)
		return (error_return("Error: Malloc failed for pipes", 1));
	
	// Create each pipe
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

/**
 * @brief Open the input file
 * 
 * @param data Main data structure
 * @param filename Input filename
 * @return int 0 on success, 1 on failure
 */
int	open_input_file(t_data *data, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: Cannot open input file");
		// If input file can't be opened, use /dev/null instead
		fd = open("/dev/null", O_RDONLY);
		if (fd < 0)
			return (1);
	}
	
	data->fd.in_file = fd;
	return (0);
}

/**
 * @brief Open the output file
 * 
 * @param data Main data structure
 * @param filename Output filename
 * @param append_mode 1 for append mode (here_doc), 0 for truncate mode
 * @return int 0 on success, 1 on failure
 */
int	open_output_file(t_data *data, char *filename, int append_mode)
{
	int	fd;
	int	flags;

	if (append_mode)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
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

/**
 * @brief Setup input and output files
 * 
 * @param data Main data structure
 * @param args Command line arguments
 * @param ac Argument count
 * @return int 0 on success, 1 on failure
 */
int	setup_files(t_data *data, char **args, int ac)
{
	char	*infile;
	int		append_mode;

	// Determine input file
	if (data->here_doc)
		infile = ".temp_here_doc.txt";
	else
		infile = args[1];
	
	// Open input file
	if (open_input_file(data, infile) != 0)
		return (1);
	
	// Determine append mode for output file
	append_mode = data->here_doc;
	
	// Open output file
	if (open_output_file(data, args[ac - 1], append_mode) != 0)
		return (clean_exit(data, 1, NULL));
	
	return (0);
}

/**
 * @brief Redirect input and output file descriptors
 * 
 * @param fd_in Input file descriptor
 * @param fd_out Output file descriptor
 */
void	redirect_io(int fd_in, int fd_out)
{
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
}