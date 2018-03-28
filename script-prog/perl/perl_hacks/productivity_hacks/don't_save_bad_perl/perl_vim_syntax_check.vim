" perl_synwrite.vim: check syntax of Perl before writing
" latest version at: http://www.vim.org/scripts/script.php?script_id=896

"" abort if b:did_perl_synwrite is true: already loaded or user pref
if exists("b:did_perl_synwrite")
  finish
endif
let b:did_perl_synwrite = 1

"" set buffer :au pref: if defined globally, inherit; otherwise, false
if (exists("perl_synwrite_au") && !exists("b:perl_synwrite_au"))
  let b:perl_synwrite_au = perl_synwrite_au
elseif !exists("b:perl_synwrite_au")
  let b:perl_synwrite_au = 0
endif

"" set buffer quickfix pref: if defined globally, inherit; otherwise, false
if (exists("perl_synwrite_qf") && !exists("b:perl_synwrite_qf"))
  let b:perl_synwrite_qf = perl_synwrite_qf
elseif !exists("b:perl_synwrite_qf")
  let b:perl_synwrite_qf = 0
endif

"" execute the given do_command if the buffer is syntactically correct perl
"" -- or if do_anyway is true
function! s:PerlSynDo(do_anyway,do_command)
  let command = "!perl -c"

  if (b:perl_synwrite_qf)
    " this env var tells Vi::QuickFix to replace "-" with actual filename
    let $VI_QUICKFIX_SOURCEFILE=expand("%")
    let command = command . " -MVi::QuickFix"
  endif

  " respect taint checking
  if (match(getline(1), "^#!.\\+perl.\\+-T") == 0)
    let command = command . " -T"
  endif

  exec "write" command

  silent! cgetfile " try to read the error file
  if !v:shell_error || a:do_anyway
    exec a:do_command
    set nomod
  endif
endfunction

"" set up the autocommand, if b:perl_synwrite_au is true
if (b:perl_synwrite_au > 0)
  let b:undo_ftplugin = "au! perl_synwrite * " . expand("%")

  augroup perl_synwrite
    exec "au BufWriteCmd,FileWriteCmd " . expand("%") . 
         " call s:PerlSynDo(0,\"write <afile>\")"
  augroup END
endif

"" the :Write command
command -buffer -nargs=* -complete=file -range=% -bang Write call \
    s:PerlSynDo("<bang>"=="!","<line1>,<line2>write<bang> <args>")
