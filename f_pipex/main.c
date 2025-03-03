/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:49:46 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/26 15:23:04 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	pipex(char	**av, char	**env)
{
	char	**cmd_a;
	char	**cmd_b;
	int		pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		return (error_pipe(1));
	cmd_a = cmd(av[1], ' ', 0);
	if (!cmd_a)
		return (error_create_cmd(1));
	cmd_b = cmd(av[2], ' ', 0);
	if (!cmd_b)
	{
		free_strs(cmd_a);
		return (error_create_cmd(1));
	}
	if (cmd_a_infile(cmd_a, env, av[0], pipe_fd) == 0)
		cmd_b_outfile(cmd_b, env, av[3], pipe_fd);
	free_strs(cmd_a);
	free_strs(cmd_b);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 5)
	{
		write(2, "Error\n", 7);
		return (1);
	}
	if (!env)
	{
		write(2, "Error : no environment\n", 25);
		return (1);
	}
	if (pipex(av + 1, env) == 1)
		return (1);
	return (0);
}
