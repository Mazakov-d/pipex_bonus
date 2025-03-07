/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 23:50:12 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:21:35 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Get the full path for a command
 * 
 * @param cmd Command name
 * @param path_dirs Array of path directories
 * @return char* Full path to the command, or NULL if not found
 */
char	*get_cmd_path(char *cmd, char **path_dirs)
{
	int		i;
	char	*full_path;
	char	**cmd_parts;

	if (!cmd || !path_dirs)
		return (NULL);
	
	// Handle absolute and relative paths
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	
	// Split command to get just the command name (without arguments)
	cmd_parts = split_c(cmd, ' ', '\0');
	if (!cmd_parts)
		return (NULL);
	
	// Try each path directory
	i = 0;
	while (path_dirs[i])
	{
		full_path = ft_strcat(path_dirs[i], cmd_parts[0]);
		if (!full_path)
		{
			free_str_array(cmd_parts);
			return (NULL);
		}
		
		// Check if executable exists
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_str_array(cmd_parts);
			return (full_path);
		}
		
		free(full_path);
		i++;
	}
	
	free_str_array(cmd_parts);
	return (NULL);
}

/**
 * @brief Set up pipe redirections based on command position
 * 
 * @param data Main data structure
 * @param cmd Current command
 */
void	setup_cmd_pipes(t_data *data, t_cmd *cmd)
{
	int	position;
	int	i;
	
	position = cmd->position;
	
	// First command: input from file, output to pipe
	if (position == 0)
	{
		dup2(data->fd.in_file, STDIN_FILENO);
		dup2(data->fd.pipes[position][1], STDOUT_FILENO);
	}
	// Last command: input from pipe, output to file
	else if (position == data->cmd_count - 1)
	{
		dup2(data->fd.pipes[position - 1][0], STDIN_FILENO);
		dup2(data->fd.out_file, STDOUT_FILENO);
	}
	// Middle commands: input and output from/to pipes
	else
	{
		dup2(data->fd.pipes[position - 1][0], STDIN_FILENO);
		dup2(data->fd.pipes[position][1], STDOUT_FILENO);
	}
	
	// Close all pipe file descriptors
	// Important: close pipes only after all dup2 calls are done
	for (i = 0; i < data->fd.pipe_count; i++)
	{
		if (data->fd.pipes[i])
		{
			close(data->fd.pipes[i][0]);
			close(data->fd.pipes[i][1]);
		}
	}
	
	// Close file descriptors
	if (data->fd.in_file > 0)
		close(data->fd.in_file);
	if (data->fd.out_file > 0)
		close(data->fd.out_file);
}

/**
 * @brief Execute a command in a child process
 * 
 * @param data Main data structure
 * @param cmd Command to execute
 * @param path_dirs Path directories
 */
void	child_process(t_data *data, t_cmd *cmd, char **path_dirs)
{
	char	*cmd_path;
	
	// Set up pipes for this command
	setup_cmd_pipes(data, cmd);
	
	// Get the full path to the command
	cmd_path = get_cmd_path(cmd->args[0], path_dirs);
	if (!cmd_path)
	{
		print_error("Command not found: ");
		print_error(cmd->args[0]);
		print_error("\n");
		exit(127);
	}
	
	// Execute the command
	if (execve(cmd_path, cmd->args, data->env) == -1)
	{
		free(cmd_path);
		print_error("Failed to execute: ");
		print_error(cmd->args[0]);
		print_error("\n");
		exit(126);
	}
}

/**
 * @brief Wait for all child processes to finish
 * 
 * @param cmd_count Number of commands
 * @param pids Array of process IDs
 * @return int Exit status of the last command
 */
int	wait_for_children(int cmd_count, pid_t *pids)
{
	int	i;
	int	status;
	int	exit_status;
	
	exit_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			// Save the exit status of the last command
			exit_status = WEXITSTATUS(status);
		}
		i++;
	}
	
	return (exit_status);
}

/**
 * @brief Run the entire pipeline
 * 
 * @param data Main data structure
 * @return int 0 on success, 1 on failure
 */
int	run_pipeline(t_data *data)
{
	t_cmd	*cmd;
	pid_t	*pids;
	char	**path_dirs;
	int		i;
	int		exit_status;
	
	// Get PATH directories from environment
	path_dirs = get_path_from_env(data->env);
	if (!path_dirs)
		return (clean_exit(data, 1, "Failed to get PATH"));
	
	// Allocate memory for process IDs
	pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!pids)
	{
		free_str_array(path_dirs);
		return (clean_exit(data, 1, "Malloc failed for PIDs"));
	}
	
	// Create child processes for each command
	cmd = data->cmd_list;
	i = 0;
	while (cmd && i < data->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			free_str_array(path_dirs);
			free(pids);
			return (clean_exit(data, 1, "Fork failed"));
		}
		else if (pids[i] == 0)
		{
			// Child process
			// Free pids as we don't need it in child
			free(pids);
			child_process(data, cmd, path_dirs);
			// Should not reach here
			exit(1);
		}
		cmd = cmd->next;
		i++;
	}
	
	// Close all file descriptors in the parent
	// This is important to avoid deadlocks
	if (data->fd.in_file > 0)
	{
		close(data->fd.in_file);
		data->fd.in_file = -1;
	}
	
	if (data->fd.out_file > 0)
	{
		close(data->fd.out_file);
		data->fd.out_file = -1;
	}
	
	// Close all pipe file descriptors in the parent
	close_all_pipes(data->fd.pipes, data->fd.pipe_count);
	
	// Wait for all child processes
	exit_status = wait_for_children(data->cmd_count, pids);
	
	// Clean up
	free_str_array(path_dirs);
	free(pids);
	
	return (exit_status);
}