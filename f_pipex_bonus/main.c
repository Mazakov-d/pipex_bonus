/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:42:43 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:21:23 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Initialize the pipex data structure
 * 
 * @param data Pointer to data structure
 * @param args Command line arguments
 * @param env Environment variables
 * @param ac Argument count
 * @return int 0 on success, 1 on failure
 */
int	init_data(t_data *data, char **args, char **env, int ac)
{
	// Initialize all fields to safe defaults
	data->env = env;
	data->here_doc = is_here_doc(args[1]);
	data->cmd_count = 0;
	data->cmd_list = NULL;
	data->fd.in_file = -1;
	data->fd.out_file = -1;
	data->fd.pipes = NULL;
	data->fd.pipe_count = 0;
	
	// Parse commands (skip first two arguments if normal mode, three if here_doc mode)
	if (data->here_doc)
		data->cmd_list = parse_commands(args + 3);
	else
		data->cmd_list = parse_commands(args + 2);
	
	if (!data->cmd_list)
		return (error_return("Error: Failed to parse commands", 1));
	
	// Count commands
	t_cmd *cmd = data->cmd_list;
	while (cmd)
	{
		data->cmd_count++;
		cmd = cmd->next;
	}
	
	// Setup file descriptors and pipes
	if (setup_files(data, args, ac) != 0)
		return (clean_exit(data, 1, NULL));
	
	if (init_pipes(data, ac, args) != 0)
		return (clean_exit(data, 1, NULL));
	
	return (0);
}

/**
 * @brief Main pipex function
 * 
 * @param args Command line arguments
 * @param env Environment variables
 * @param ac Argument count
 * @return int 0 on success, 1 on failure
 */
int	pipex(char **args, char **env, int ac)
{
	t_data	data;

	// Initialize data structure
	if (init_data(&data, args, env, ac) != 0)
		return (1);
	
	// Run the pipeline
	if (run_pipeline(&data) != 0)
		return (clean_exit(&data, 1, NULL));
	
	// Clean up and exit
	return (clean_exit(&data, 0, NULL));
}

/**
 * @brief Main function
 * 
 * @param ac Argument count
 * @param av Argument values
 * @param env Environment variables
 * @return int 0 on success, 1 on failure
 */
int	main(int ac, char **av, char **env)
{
	int	here_doc_status;
	int	exit_status;

	// Check argument count
	if (ac < 5 || (ac < 6 && is_here_doc(av[1])))
		return (error_return("Error: Invalid number of arguments", 1));
	
	// Setup here_doc if needed
	if (is_here_doc(av[1]))
	{
		here_doc_status = setup_here_doc(av + 2);
		if (here_doc_status == -1)
			return (1);
		
		// Run pipex
		exit_status = pipex(av, env, ac);
		
		// Remove temporary here_doc file
		unlink(".temp_here_doc.txt");
		
		return (exit_status);
	}
	else
	{
		// Run pipex in normal mode
		exit_status = pipex(av, env, ac);
		return (exit_status);
	}
}