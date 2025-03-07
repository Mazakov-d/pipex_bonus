/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:42:43 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:14:07 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	pipex(char **args, char **env, int ac)
{
	t_data	*data;

	data = get_commands(args + 2);
	if (!data)
	{
		write(2, "Error : Creating data\n", 23);
		return (1);
	}
	if (open_in_out(data, args, ac) == 1)
		return (free_cmd_fd(data, 1, NULL));
	
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
		
}
