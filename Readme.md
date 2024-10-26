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
