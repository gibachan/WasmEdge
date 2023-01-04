CC = gcc

all: host_function memory.wasm

host_function: memory.c
	$(CC) $^ -ljson-c -lwasmedge -o $@

%.wasm: %.wat
	wat2wasm --enable-all $^ -o $@

clean:
	rm -f host_function parse-json.wasm
