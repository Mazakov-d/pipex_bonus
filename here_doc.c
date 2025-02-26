/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:02:19 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/26 16:11:38 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	here_doc_error(int i)
{
	write(2, "Error : can't create temp_here_doc", 35);
	return (i);
}

int	is_here_doc(char *str)
{
	if (ft_strlen(str) == 8)
	{
		if (str[0] == 'h' && str[1] == 'e' && str[2] == 'r' && str[3] == 'e'
			&& str[4] == '_' && str[5] == 'd' && str[6] == 'o' && str[7] == 'c')
			return (1);
	}
	return (0);
}

int	is_limiter(char *str, char *limiter)
{
	int	i;

	i = 0;
	while (str && str[i] && limiter && limiter[i] && str[i] == limiter[i])
		i++;
	if (limiter[i] == 0 && str[i] == '\n')
		return (0);
	return (1);
}

int	here_doc(char **args)
{
	char	*line;
	int		fd;

	if (is_here_doc(args[0]) == 0)
		return (0);
	fd = open(".temp_here_doc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		return (here_doc_error(-1));
	line = get_next_line(0);
	while (line)
	{
		if (!line)
		{
			close(fd);
			return (-1);
		}
		if (is_limiter(line, args[1]) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	close(fd);
	return (1);
}
