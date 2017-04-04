set foldenable          " 允许折叠
set foldmethod=manual   " 手动折叠
set syntax=on           " 语法高亮
set softtabstop=4
set shiftwidth=4
set tabstop=4           " Tab键的宽度
set cindent             " C风格缩进
set number              " 显示行号
set hlsearch            " 搜索逐字符高亮
set showmatch           " 高亮显示匹配的括号
set matchtime=2         " 匹配括号高亮的时间（单位是十分之一秒）

" vim重新载入时，继续上次的光标位置 2015-8-17
if has("autocmd")
  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

" 高亮显示当前行，去掉CursorLine和CursorColumn只会下划线方式高亮
" ctermbg设置背景颜色, ctermfg设置前景颜色
set cursorline			" 突出显示当前行
hi CursorLine  cterm=NONE  ctermbg=NONE ctermfg=green" 以颜色方式高亮当前行颜色可以修改
hi CursorColumn cterm=NONE ctermbg=NONE ctermfg=green" 颜色可以颜色同上，否则报错

" inoremap第一个参数为左操作符, 第二个参数为右操作符
" 使用键映射设置括号补全(),[],{},"",''
inoremap ( ()<ESC>i
inoremap [ []<ESC>i
inoremap { {}<ESC>i
inoremap " ""<ESC>i
inoremap ' ''<ESC>i
