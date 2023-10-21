#include "get_next_line.h"

int		ft_strlen(char	*str) //contar as letras da string
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2) //juntar duas strings
{
	char	*str;
	int	i = -1;
	int	j = -1;
	
	str = malloc(sizeof(char) * (ft_strlen(s2) + ft_strlen(s1) + 1));
	if (!str)
		return NULL;
	while(s1[++i])
		str[i] = s1[i];
	while(s2[++j])
		str[i++] = s2[j];
	str[i] = '\0';
	return (str);
}

int		is_nl(char *line) //verificar se tem \n
{
	int	i = -1;

	while (line[++i])
	{
		if (line[i] == '\n')
			return (1);
	}
	return (0);
}

char	*ft_extract(char *backup)
{
	int	i = 0;
	int j = 0;
	char *line;

	if (!backup)// Verificar se 'backup' é NULL.
		return NULL;
	while (backup[i] != '\n' && backup[i])// Encontrar o final da primeira linha ou o final da string em 'backup'.
		i++;
	if (!(line = malloc((i + 2) * sizeof(char))))// Alocar memória para 'line' com espaço para a linha extraída e o caractere nulo.
		return NULL;

	i = -1;// Reiniciar 'i' para -1 e copiar os caracteres da primeira linha para 'line'.
	
	while (backup[++i] != '\n' && backup[i])//passar o que tem no backup para a line.
		line[i] = backup[i];

	if (backup[i] == '\n') // Se uma quebra de linha foi encontrada, copie-a para 'line'. 
	{
		line[i] = backup[i];
		i++;
	}
	line[i] = '\0';
	while (backup[i])// Ajustar 'backup' para que ele comece na próxima linha (se houver).
		backup[j++] = backup[i++];
	backup[j] = '\0';// Adicionar o caractere nulo no final da nova string em 'backup'.
	return (line);// Retornar o ponteiro para a linha extraída.
}


char	*ft_read(int fd, char *line, char *backup)
{
	int	bytesRead = 1;
	char	*temp;

	while(bytesRead)
	{
		if (!backup)
		{
			if (!(backup = malloc(1 * sizeof(char))))
				return NULL;
			backup[0] = '\0';
		}
		if(is_nl(backup))
			break;
		bytesRead = read(fd, line, BUFFER_SIZE);
		if (bytesRead == -1)
			return (free(backup), NULL);
		if (bytesRead == 0)
			break;
		line[bytesRead] = '\0';
		temp = backup;
		backup = ft_strjoin(backup, line);
		free(temp);
	}
	return (backup);
}

char	*get_next_line(int fd)
{
	char	*line;
	static char	*backup;

	if (fd < 0)
		return NULL;
	line = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if(!line)
		return NULL;
	backup = ft_read(fd, line, backup);
	free(line);
	line = NULL;
	line = ft_extract(backup);
	if (backup && ft_strlen(backup) == 0)
	{
		free(backup);
		backup = NULL;
	}
	return(line);	
}

int	main()
{
	int fd;
	int i = -1;
	char	*line;

	fd = open("teste.txt", O_RDONLY);
	while(++i < 10)
	{
		line = get_next_line(fd);
		if (line)
		{
			printf("%s", line);
			fflush(stdout);
		}
		free(line);
	}
}