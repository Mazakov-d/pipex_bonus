/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:06:35 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/03 20:35:09 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**cmd(char *c, int a, int b)
{
	char	**cmd;

	cmd = split_c(c, a, b);
	if (!cmd)
		return (NULL);
	return (cmd);
}

int	open_switch_stdin(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		write(2, "Error : can't read infile\n", 27);
		return (1);
	}
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

int	cmd_a_infile(char **cmd_a, char **env, char *infile, int pipe_fd[2])
{
	int		pid;
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd_a[0], get_path_env(env));
	if (!path_cmd)
		return (1);
	if (open_switch_stdin(infile) == 1)
		return (free_close_fd(path_cmd, NULL, 1));
	pid = fork();
	if (pid < 0)
		return (error_fork(1, path_cmd, NULL));
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
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	free(path_cmd);
	return (0);
}

int	cmd_b_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2])
{
	int		pid;
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd_b[0], get_path_env(env));
	if (!path_cmd)
		return (free_close_fd(NULL, pipe_fd, 1));
	if (open_switch_stdout(outfile) == 1)
		return (free_close_fd(path_cmd, pipe_fd, 1));
	pid = fork();
	if (pid < 0)
		return (error_fork(1, path_cmd, pipe_fd));
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
	close(pipe_fd[0]);
	waitpid(pid, NULL, 0);
	free(path_cmd);
	return (0);
}
