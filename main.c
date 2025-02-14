/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:01:58 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/14 14:51:24 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"



int	pipex(char **args, char **env)
{
	t_cmd	*cmds;
	int		pipe_fd[2];
	int		i;

	cmds = get_commands(args);
	if (!cmds)
		return (1);
	if (pipe(pipe_fd) < 0)
		return (free_cmds_fd(cmds, pipe_fd, 1));
	if (cmd_infile(cmds[1].cmd, env, args[0], pipe_fd) == 1)
		return (free_cmds_fd(cmds, pipe_fd, 1));
	i = 1;
	while (cmds[i].cmd && cmds[i + 1].cmd && cmds[i + 2].cmd)
	{
		if (cmd_to_pipe(cmds[i].cmd, env,pipe_fd) == 1)
			return (free_cmds_fd(cmds, pipe_fd, 1));
		i++;
	}
	if (cmd_outfile(cmds[i].cmd, env, cmds[i + 1].cmd[0], pipe_fd) == 1)
		return (free_cmds_fd(cmds, pipe_fd, 1));
	return (free_cmds_fd(cmds, pipe_fd, 0));
}

int main(int ac, char **av, char **env)
{
	if (ac > 2)
	{
		if (pipex(av + 1, env) == 1)
			write(2, "Error\n", 7);
	}
}