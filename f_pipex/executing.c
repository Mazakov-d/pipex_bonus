/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:06:35 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 17:45:16 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**create_cmd(char *c, int a, int b)
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
		perror("Error: Cannot open infile");
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

int	cmd_a_infile(t_cmd *cmd, char **env, char *infile, int pipe_fd[2])
{
	int			pid;
	t_pipe_data	data;

	pid = fork();
	if (pid < 0)
		return (error_fork(1, NULL, pipe_fd));
	if (pid == 0)
	{
		data.pipe_fd[0] = pipe_fd[0];
		data.pipe_fd[1] = pipe_fd[1];
		data.infile = infile;
		child_process_first(cmd, env, data);
	}
	close(pipe_fd[1]);
	return (0);
}

int	cmd_b_outfile(t_cmd *cmd, char **env, char *outfile, int pipe_fd[2])
{
	int			pid;
	int			status;
	t_pipe_data	data;

	pid = fork();
	if (pid < 0)
		return (error_fork(1, NULL, pipe_fd));
	if (pid == 0)
	{
		data.pipe_fd[0] = pipe_fd[0];
		data.pipe_fd[1] = pipe_fd[1];
		data.outfile = outfile;
		child_process_second(cmd, env, data);
	}
	close(pipe_fd[0]);
	waitpid(pid, &status, 0);
	waitpid(-1, NULL, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}
