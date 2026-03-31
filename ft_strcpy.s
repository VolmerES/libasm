;
; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_strcpy.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: volmer <volmer@student.42.fr>              +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2026/03/25 13:10:56 by volmer            #+#    #+#              ;
;    Updated: 2026/03/25 15:03:21 by volmer           ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

section .text
	global _ft_strcpy

_ft_strcpy:
	; rdi = dst, rsi = src
	; strcpy devuelve el puntero al destino
	mov rax, rdi
	; rcx sera el indice de copia
	mov rcx, 0

loop:
	; Carga el byte actual de src en dl
	mov dl, byte [rsi + rcx]
	; Escribe ese mismo byte en dst
	mov byte [rdi + rcx], dl
	; Incrementa el indice
	inc rcx
	; Comprueba si el byte copiado era el terminador nulo
	test dl, dl ;Test con operador and de bits, si dl es 0, actualiza flag ZF = 1
	; Si el byte era 0, ZF se establece a 1 y salta a done
	jz done
	; Si no, continúa copiando el siguiente byte
	jmp loop
	
	

done:
	ret
	