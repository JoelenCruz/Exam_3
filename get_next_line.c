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

// Essa função é  projetada para extrair a primeira linha de texto de uma string, 
// copiá-la para uma nova string alocada dinamicamente (chamada line) e, em seguida, ajustar a string original (backup) 
// para que ela comece na próxima linha (se houver).
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

	i = 0;// Reiniciar 'i' para 0 e copiar os caracteres da primeira linha para 'line'.
	
	while (backup[i] != '\n' && backup[i])//passar o que tem no backup para a line.
	{
		line[i] = backup[i];
		i++;
	}

	if (backup[i] == '\n') // Se uma quebra de linha foi encontrada, copie-a para 'line'. 
	{
		line[i] = backup[i];
		i++;
	}
	line[i] = '\0'; //adicionar o byte nulo em line

	while (backup[i])// Ajustar 'backup' para que ele comece na próxima linha (se houver).
	{
		backup[j] = backup[i];
		j++;
		i++;
	}

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
			if (!(backup = malloc(1 * sizeof(char))))  /* Aloca memória para backup, inicializado com uma string vazia */
				return NULL;
			backup[0] = '\0';/* Define o primeiro caractere de backup como nulo para criar uma string vazia. */
		}
		if(is_nl(backup))/* Verifica se há uma nova linha em backup */
			break;
		bytesRead = read(fd, line, BUFFER_SIZE);/* Lê dados do descritor de arquivo 'fd' para 'line' */
		if (bytesRead == -1)//caso o read retorn -1
			return (free(backup), NULL); /* Libera a memória alocada para backup e retorna NULL */
		if (bytesRead == 0)/* Verifica se chegou ao final do arquivo */
			break;
		line[bytesRead] = '\0'; /* Adiciona um caractere nulo ao final de 'line' para criar uma string válida. */
		temp = backup; /* Salva o valor atual de backup em temp */
		backup = ft_strjoin(backup, line);/* Concatena backup com line, provavelmente alocando memória dinamicamente. */
		free(temp); /* Libera a memória anteriormente alocada para backup */
	}
	return (backup); /* Retorna o ponteiro para a string completa lida do arquivo. */
}

char	*get_next_line(int fd)
{
	char	*line;
	static char	*backup;

	if (fd < 0) // Verifica se o descritor de arquivo 'fd' é válido (não negativo).
		return NULL;
	if(!(line = malloc(sizeof(char) * (BUFFER_SIZE + 1)))) // Aloca memória para o buffer 'line' com tamanho 'BUFFER_SIZE + 1'.
		return NULL; // Verifica se a alocação de memória para 'line' foi bem-sucedida. Se não, retorna NULL.
	backup = ft_read(fd, line, backup);// Chama a função 'ft_read' para ler dados do arquivo 'fd' e armazená-los em 'backup'.
	free(line);// Libera a memória alocada para 'line', pois os dados já foram copiados para 'backup'.
	line = NULL;// Define 'line' como NULL para evitar possíveis problemas de acesso posterior.
	line = ft_extract(backup); // Chama a função 'ft_extract' para extrair a próxima linha de 'backup'.
	if (backup && ft_strlen(backup) == 0) // Verifica se 'backup' está vazio (contém somente '\0').
	{
		free(backup); // Libera a memória alocada para 'backup'.
		backup = NULL; // Define 'backup' como NULL para indicar que não há mais dados não lidos.
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
