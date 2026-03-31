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
	global _ft_write
	extern __error ; Declaración de la función de error, asumiendo que existe en otro archivo

_ft_write:
	; rdi = fd, rsi = buf, rdx = count
	mov rax, 0x2000004 ; numero syscall para sys_write en macOS
	; Validación de argumentos, aunque ya la haga el kernel
	cmp rdi, 0; si el fd es -1, no es válido, devuelve -1
	jl error ; jl = jump if less, salta a error si rdi < 0

	syscall ; realiza la llamada al sistema, los argumentos ya están en rdi, rsi y rdx
	jc error ; jc = jump if carry, si la syscall devuelve un error, el flag de carry se establece, salta a error
	jge done ; si rax >= 0, salta a done jgc = jump if greater, salta a done si rax > 0

error:
	mov r8, rax ; devuelve -1 en caso de error
	sub rsp, 8 ; reserva espacio en la pila para el valor de retorno
	call __error ; llama a la función de error para imprimir el mensaje
	add rsp, 8 ; limpia la pila después de la llamada a error
	mov dword [rsp], r8d ; coloca el valor de retorno (-1) en la pila para devolverlo
	mov rax, -1 ; devuelve -1 en caso de error
	jmp done ; salta a done para retornar el valor en rax

done:
	ret ; retorna el número de bytes escritos o -1 en caso de error