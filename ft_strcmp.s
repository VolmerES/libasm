;
; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_strlen.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: volmer <volmer@student.42.fr>              +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2026/03/25 13:10:56 by volmer            #+#    #+#              ;
;    Updated: 2026/03/25 15:03:21 by volmer           ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

section .text
	global _ft_strcmp

_ft_strcmp:
	; rdi = s1, rsi = s2
	mov rcx, 0

loop:
	; Carga el byte actual de cada string
	mov al, byte [rdi + rcx]
	mov dl, byte [rsi + rcx]

	; Si son distintos, devuelve (unsigned char)al - (unsigned char)dl
	cmp al, dl
	jne diff ;jne = jump if not equal, salta a diff si al != dl

	; Comprueba si hemos llegado al final del primer string
	test al, al
	je equal

	inc rcx
	jmp loop

diff:
	movzx eax, al ;movzx = move with zero-extend, convierte al a un entero sin signo de 32 bits
	movzx edx, dl ;convierte dl a un entero sin signo de 32 bits
	sub eax, edx  ;subs = subtract, eax = eax - edx
	ret

equal:
	xor eax, eax
	ret