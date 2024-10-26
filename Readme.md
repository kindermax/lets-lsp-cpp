# Lsp server for lets task runner (C++ version for educational purposes)

## Supported features

* [x] Goto definition
* [ ] Completion
* [ ] Diagnostics
* [ ] Hover
* [ ] Document highlight
* [ ] Document symbol
* [ ] Formatting
* [ ] Signature help
* [ ] Code action

## Development

On `linux` install build dependencies:

```bash
apt-get install -y \
  build-essential \
  cmake \
  git
```

Optional dependencies:

```bash
apt-get intstall -y clang-tidy  # need 18 version
```

On `macos` install build dependencies:

```bash
brew install cmake
brew install git
```

Optional dependencies:

`clang-tidy` and `clang-format` are not available directly via brew, but we can install them via llvm:

```bash
brew install llvm
```

Next we need to add `clang-tidy` and `clang-format` to the `PATH`:

```bash
# or change BIN_DIR to your own bin dir
BIN_DIR=/usr/local/bin
ln -s "$(brew --prefix llvm)/bin/clang-tidy" "${BIN_DIR}"
ln -s "$(brew --prefix llvm)/bin/clang-format" "${BIN_DIR}"
```

### Build and run

```bash
cmake -S . -B build
cmake --build build
./build/lets_ls --file /path/to/log.txt
```

### Testing

```bash
cmake -S . -B build
cmake --build build

ctest --test-dir build --output-on-failure 
```

## Use in neovim

Add new filetype:

```lua
vim.filetype.add({
  filename = {
    ["lets.yaml"] = "yaml.lets",
  },
})
```

In your `neovim/nvim-lspconfig` servers configuration:

In order for nvim-lspconfig to recognize lets_lsp we must define config for `lets_ls`

```lua
require("lspconfig.configs").lets_ls = {
  default_config = {
    cmd = { 
      "/Users/max/code/projects/lets-ls-cpp/build/lets_ls",
      "--file",
      "/Users/max/code/projects/lets-ls-cpp/log.txt",
    },
    filetypes = { "yaml.lets" },
    root_dir = util.root_pattern("lets.yaml"),
    settings = {},
  },
}
```

And then enable `lets_ls` in then `servers` section:

```lua
return {
  "neovim/nvim-lspconfig",
  opts = {
    servers = {
      lets_ls = {},
      pyright = {},  -- just to show an example how we enable lsp servers
    },
  },
}
```
