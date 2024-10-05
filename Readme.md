# Lsp server for lets task runner (C++ version for educational purposes)


## Development

### Build and run

```bash
cmake -S . -B build
cmake --build build
./build/lets-lsp
```

### Testing

```bash
cmake -S . -B build
cmake --build build

ctest --test-dir build --output-on-failure 
```
