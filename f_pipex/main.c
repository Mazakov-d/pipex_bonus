/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:49:46 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 17:46:47 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>      

void	cleanup_resources(char **cmd_a, char **cmd_b)
{
	if (cmd_a)
		free_strs(cmd_a);
	if (cmd_b)
		free_strs(cmd_b);
}

int	pipex(char	**av, char	**env)
{
	t_cmd	cmd;
	int		pipe_fd[2];
	int		status;

	if (pipe(pipe_fd) < 0)
		return (error_pipe(1));
	cmd.cmd_a = create_cmd(av[1], ' ', 0);
	if (!cmd.cmd_a)
		return (error_create_cmd(pipe_fd, 1));
	cmd.cmd_b = create_cmd(av[2], ' ', 0);
	if (!cmd.cmd_b)
	{
		free_strs(cmd.cmd_a);
		return (error_create_cmd(pipe_fd, 1));
	}
	cmd_a_infile(&cmd, env, av[0], pipe_fd);
	status = cmd_b_outfile(&cmd, env, av[3], pipe_fd);
	free_cmd(&cmd);
	printf("%d\n", status);
	return (status);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 5)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		write(2, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 41);
		return (1);
	}
	if (!env)
	{
		write(2, "Error: No environment\n", 23);
		return (1);
	}
	return (pipex(av + 1, env));
}
