#include <stdarg.h> // For va_lis, va_start, va_arg, va_copy, va_end
#include <unistd.h> // For malloc, free, write
  

void	put_string(char *string, int *len)
{
	if (!string)
		string = "(null)";
	while (*string)
		*len += write(1, string++, 1);
}

void	put_digit(long long int number, int base, int *len)
{
	char	*hexa = "0123456789abcdef";

	if (number < 0)//tratamento de nagativos
	{
		number *= -1;//passar para positivo 
		*len += write(1, "-", 1);//imprimir o sinal negativo e alimentar a varaivel len
	}
	if (number >= base)
		put_digit((number / base), base, len);//recursividade para tentar imprimir o numero divido pela base
	*len += write(1, &hexa[number % base], 1);//escreve o numero dentro da sua base
	//alimentar o len
}

int	ft_printf(const char *format, ...)
{
	int			len;
	va_list		ptr;//Declara uma variável ptr do tipo va_list, que é usada para acessar os argumentos variáveis passados para a função.

	len = 0;
	va_start(ptr, format);//Inicializa a variável ptr para apontar para o primeiro argumento variável após format. 
	//Esta macro é necessária para acessar os argumentos variáveis.
	while (*format)
	{
		if ((*format == '%') && *(format + 1))
		{
			format++;
			if (*format == 's')
				put_string(va_arg(ptr, char *), &len);
			else if (*format == 'd')
				put_digit((long long int)va_arg(ptr, int), 10, &len);
			else if (*format == 'x')
				put_digit((long long int)va_arg(ptr, unsigned int), 16, &len);
		}
		else
			len += write(1, format, 1);
		format++;
	}
	return (va_end(ptr), len);
	///Fecha a lista de argumentos variáveis com va_end e retorna o valor atualizado de len, 
	//que representa o número total de caracteres escritos.
}

#include<stdio.h>
#include <stdio.h>

int main(void)
{
    char string[] = "New String";
    int returnlen;

    returnlen = ft_printf("Simple Print\n");
    printf("Len Simple Print: %d\n", returnlen);

    returnlen = printf("Simple Print\n");
    printf("Len Simple Print: %d\n", returnlen);

    printf("------------------------\n");

    printf("------------------------\n");

    returnlen = ft_printf("String: %d\n", 3000);
    printf("Len String: %d\n", returnlen);

    returnlen = printf("String: %d\n", 3000);
    printf("Len String: %d\n", returnlen);

    return 0;
}