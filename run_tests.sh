#!/usr/bin/env bash
set -euo pipefail

if [ -t 1 ]; then
	C_RESET='\033[0m'
	C_GREEN='\033[32m'
	C_CYAN='\033[36m'
	C_YELLOW='\033[33m'
else
	C_RESET=''
	C_GREEN=''
	C_CYAN=''
	C_YELLOW=''
fi

step() {
	printf "%b[%s]%b %s\n" "$C_CYAN" "$1" "$C_RESET" "$2"
}

mini_anim() {
	if [ -t 1 ] && [ "${NO_ANIM:-0}" != "1" ]; then
		printf "%b   %s%b" "$C_YELLOW" "working" "$C_RESET"
		for _ in 1 2 3; do
			printf "."
			sleep 0.07
		done
		printf "\n"
	fi
}

# Always clean build artifacts after tests (success or failure).
cleanup() {
	make fclean >/dev/null || true
}
trap cleanup EXIT

CC_BIN="${CC:-cc}"
CFLAGS="-Wall -Wextra -Werror"
SAN_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
USE_SANITIZER="${USE_SANITIZER:-1}"

step "1/3" "Building libasm"
mini_anim
make fclean >/dev/null
make all >/dev/null

step "2/3" "Building test binary"
mini_anim
if [ "$USE_SANITIZER" = "1" ]; then
	"$CC_BIN" $CFLAGS $SAN_FLAGS main.c libasm.a -o test_program
else
	"$CC_BIN" $CFLAGS main.c libasm.a -o test_program
fi

step "3/3" "Running tests"
./test_program
printf "%b[OK]%b Test run finished\n" "$C_GREEN" "$C_RESET"
