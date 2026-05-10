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
	global _ft_strdup
	extern _malloc
	extern _ft_strlen
	extern _ft_strcpy

_ft_strdup:
	; rdi s1
	push rdi
	call _ft_strlen
	inc rax

	mov rdi, rax
	call _malloc

	cmp rax, 0
	je error_malloc
	
	mov rdi, rax
	pop rsi

	call _ft_strcpy

	ret

error_malloc:
	pop rdi
	mov rax, 0
	ret