/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:02:38 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/16 14:00:28 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	cmd_infile(char **cmd_a, char **env, char *infile, int pipe_fd[2])
{
	int		pid;
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd_a[0], get_path_env(env));
	if (!path_cmd)
		return (1);
	if (open_switch_stdin(infile) == 1)
		return (free_ptr(path_cmd, 1));
	pid = fork();
	if (pid < 0)
		return (free_ptr(path_cmd, 1));
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		execve(path_cmd, cmd_a, env);
		free(path_cmd);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	// close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	return (free_ptr(path_cmd, 0));
}

int	cmd_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2])
{
	int		pid;
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd_b[0], get_path_env(env));
	if (!path_cmd)
		return (1);
	if (open_switch_stdout(outfile) == 1)
		return (free_ptr(path_cmd, 1));
	pid = fork();
	if (pid < 0)
		return (free_ptr(path_cmd, 1));
	if (pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		execve(path_cmd, cmd_b, env);
		free(path_cmd);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	waitpid(pid, NULL, 0);
	return (free_ptr(path_cmd, 0));
}

int	cmd_to_pipe(char **cmd, char **env, int pipe_fd[2])
{
	int		pid;
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd[0], get_path_env(env));
	if (!path_cmd)
		return (1);
	switch_fd_in_out(pipe_fd[0], pipe_fd[1]);
	pid = fork();
	if (pid < 0)
		return (free_ptr(path_cmd, 1));
	if (pid == 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execve(path_cmd, cmd, env);
		free(path_cmd);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
	return (free_ptr(path_cmd, 0));
}
