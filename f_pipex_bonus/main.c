/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:42:43 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:40:04 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	set_pipe(t_data *data, int ac, char **args)
{
	int	i;

	i = 0;
	if (is_here_doc(args[1]))
		data->pipe_fd = malloc(sizeof(int *) * (ac - 4));
	else
		data->pipe_fd = malloc(sizeof(int *) * (ac - 3));
	if (!data->pipe_fd)
		return (free_data_fd(data, 1, "Error: malloc\n"));
	while (data->pipe_fd[i])
	{
		data->pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!data->pipe_fd[i])
			return (free_data_fd(data, 1, "Error: malloc\n"));
		if (pipe(data->pipe_fd[i]) == -1)
            return (free_data_fd(data, 1, "Error: pipe\n"));
		i++;
	}
	return (0);
}

int	pipex(char **args, char **env, int ac)
{
	t_data	*data;

	(void)env;
	data = get_commands(args + 2);
	if (!data)
	{
		write(2, "Error : Creating data\n", 23);
		return (1);
	}
	if (open_in_out(data, args, ac) == 1 || set_pipe(data, ac, args) == 1)
		return (1);
	int i = 0;
	while (data->next)
	{
		printf("cmd : %s, i : %d\n", data->cmd[0], i);
		data = data->next;
		i++;
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int	save;

	if (ac < 5 || (ac < 6 && is_here_doc(av[1])))
	{
		write(2, "Error : invalid numbers of arguments\n", 38);
		return (1);
	}
	if (is_here_doc(av[1]) == 1)
	{
		save = here_doc(av + 2);
		if (save == -1)
			return (1);
		pipex(av, env, ac);
		unlink(".temp_here_doc.txt");
	}
	else
		pipex(av, env, ac);
}
