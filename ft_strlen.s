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
	global _ft_strlen

_ft_strlen:
	mov rax, 0

loop:
	cmp byte [rdi + rax], 0
	je done
	inc rax
	jmp loop

done:
	ret