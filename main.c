/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:53:39 by volmer            #+#    #+#             */
/*   Updated: 2026/03/25 14:53:41 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

// Declaras que la función existe fuera de este archivo
extern size_t ft_strlen(const char *s);

int main(void)
{
    char *test = "Hola 42";
    
    printf("Original strlen: %zu\n", strlen(test));
    printf("Tu ft_strlen:     %zu\n", ft_strlen(test));
    
    if (strlen(test) == ft_strlen(test))
        printf("✅ ¡Funciona!\n");
    else
        printf("❌ Error en el conteo\n");

    return (0);
}