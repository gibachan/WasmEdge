# Parse JSON example with WasmEdge C API

The `parse-json` example shows how to use a host function with a shared library `json-c`. The `parseJson` function accepts a string as a key, parses the JSON file `test.json` and returns the corresponding value.

The `parse-json.wat` is a WebAssembly script. It is compiled into WebAssembly in the form of `parse-json.wasm` using the [WABT tool](https://github.com/WebAssembly/wabt).

## Instructions

```bash
# Compilation
$ gcc memory.c -lwasmedge -o memory
$ ./memory
Got the result: Success
```
