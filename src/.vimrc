set makeprg=make\ -C\ ../devel\ -j3

" clang format
map <C-K> :pyf /usr/share/vim/addons/syntax/clang-format-3.8.py<cr>
imap <C-K> <c-o>:pyf /share/vim/addons/syntax/clang-format-3.8.py<cr>

filetype plugin indent on
set tabstop=2
set shiftwidth=2
set expandtab
set softtabstop=2
