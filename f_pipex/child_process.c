/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:30:20 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 15:02:00 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process_first(char **cmd, char **env, t_pipe_data data)
{
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd[0], get_path_env(env));
	close(data.pipe_fd[0]);
	if (open_switch_stdin(data.infile) == 1)
	{
		close(data.pipe_fd[1]);
		if (path_cmd)
			free(path_cmd);
		exit(EXIT_FAILURE);
	}
	dup2(data.pipe_fd[1], STDOUT_FILENO);
	close(data.pipe_fd[1]);
	if (path_cmd)
		execve(path_cmd, cmd, env);
	perror("Error: Command not found");
	if (path_cmd)
		free(path_cmd);
	exit(EXIT_FAILURE);
}

void	child_process_second(char **cmd, char **env, t_pipe_data data)
{
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd[0], get_path_env(env));
	close(data.pipe_fd[1]);
	if (open_switch_stdout(data.outfile) == 1)
	{
		close(data.pipe_fd[0]);
		if (path_cmd)
			free(path_cmd);
		exit(EXIT_FAILURE);
	}
	dup2(data.pipe_fd[0], STDIN_FILENO);
	close(data.pipe_fd[0]);
	if (path_cmd)
		execve(path_cmd, cmd, env);
	perror("Error: Command not found");
	if (path_cmd)
		free(path_cmd);
	exit(EXIT_FAILURE);
}
