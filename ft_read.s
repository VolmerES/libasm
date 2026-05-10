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
	global _ft_read
	extern ___error

_ft_read:
	mov rax, 0x2000003
	syscall
	jc error
	ret


error:
	push rax
	call ___error
	pop rdx
	mov [rax], rdx
	mov rax, -1
	ret
