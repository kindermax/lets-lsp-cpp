# Lsp server for lets task runner (C++ version for educational purposes)


## Development

### Build and run

```bash
cmake -S . -B build
cmake --build build
./build/lets-ls
```

### Testing

```bash
cmake -S . -B build
cmake --build build

ctest --test-dir build --output-on-failure 
```

## Use in neovim

Add new filetype:

```
vim.filetype.add({
  filename = {
    ["lets.yaml"] = "yaml.lets",
  },
})
```

In your `neovim/nvim-lspconfig` servers configuration:

In order for nvim-lspconfig to recognize lets_lsp we must define config for `lets_ls`

```
require("lspconfig.configs").lets_ls = {
  default_config = {
    cmd = { "/Users/max/code/projects/lets-ls-cpp/build/lets-ls" },
    filetypes = { "yaml.lets" },
    root_dir = util.root_pattern("lets.yaml"),
    settings = {},
  },
}
```

And then enable `lets_ls` in then `servers` section:

```
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