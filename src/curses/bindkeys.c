#ifndef	NO_IDENT
static	char	Id[] = "$Id: bindkeys.c,v 12.3 1994/07/20 00:12:34 tom Exp $";
#endif

/*
 * Title:	bindkeys.c
 * Author:	T.E.Dickey
 * Created:	02 Jul 1994
 * Modified:
 *
 * Function:	provide a key-binding facility for curses applications.
 */
#if 0
             VI Insert Mode functions

             "^@"                     ->  is undefined
             "^A"                     ->  beginning-of-line
             "^B"                     ->  backward-char
             "^C"                     ->  tty-sigintr
             "^D"                     ->  list-or-eof
             "^E"                     ->  end-of-line
             "^F"                     ->  forward-char
             "^G"                     ->  list-glob
             "^H"                     ->  backward-delete-char
             "^I"                     ->  complete-word
             "^J"                     ->  newline
             "^K"                     ->  kill-line
             "^L"                     ->  clear-screen
             "^M"                     ->  newline
             "^N"                     ->  down-history
             "^O"                     ->  tty-flush-output
             "^P"                     ->  up-history
             "^Q"                     ->  tty-start-output
             "^R"                     ->  redisplay
             "^S"                     ->  tty-stop-output
             "^T"                     ->  transpose-chars
             "^U"                     ->  backward-kill-line
             "^V"                     ->  quoted-insert
             "^W"                     ->  backward-delete-word
             "^X"                     ->  expand-line
             "^Y"                     ->  tty-dsusp
             "^Z"                     ->  tty-sigtsusp
             "^["                     ->  vi-cmd-mode
             "^\"                     ->  tty-sigquit
             " " to "~"               ->  self-insert-command
             "^?"                     ->  backward-delete-char

             VI Command Mode functions

             "^@"                     ->  is undefined
             "^A"                     ->  beginning-of-line
             "^B"                     ->  is undefined
             "^C"                     ->  tty-sigintr
             "^D"                     ->  list-choices
             "^E"                     ->  end-of-line
             "^F"                     ->  is undefined
             "^G"                     ->  list-glob
             "^H"                     ->  backward-char
             "^I"                     ->  vi-cmd-mode-complete
             "^J"                     ->  newline
             "^K"                     ->  kill-line
             "^L"                     ->  clear-screen
             "^M"                     ->  newline
             "^N"                     ->  down-history
             "^O"                     ->  tty-flush-output
             "^P"                     ->  up-history
             "^Q"                     ->  tty-start-output
             "^R"                     ->  redisplay
             "^S"                     ->  tty-stop-output
             "^T"                     ->  is undefined
             "^U"                     ->  backward-kill-line
             "^V"                     ->  is undefined
             "^W"                     ->  backward-delete-word
             "^X"                     ->  expand-line
             "^["                     ->  sequence-lead-in
             "^\"                     ->  tty-sigquit
             " "                      ->  forward-char
             "!"                      ->  expand-history
             "$"                      ->  end-of-line
             "*"                      ->  expand-glob
             "+"                      ->  down-history
             ";"                      ->  vi-repeat-char-fwd
             ","                      ->  vi-repeat-char-back
             "-"                      ->  up-history
             "."                      ->  is undefined
             "/"                      ->  vi-search-back
             "0"                      ->  vi-zero
             "1" to "9"               ->  digit-argument
             "?"                      ->  vi-search-fwd
             "@"                      ->  is undefined
             "A"                      ->  vi-add-at-eol
             "B"                      ->  vi-word-back
             "C"                      ->  change-till-end-of-line
             "D"                      ->  kill-line
             "E"                      ->  vi-endword
             "F"                      ->  vi-char-back
             "I"                      ->  vi-insert-at-bol
             "J"                      ->  history-search-forward
             "K"                      ->  history-search-backward
             "N"                      ->  vi-search-back
             "O"                      ->  sequence-lead-in
             "R"                      ->  vi-replace-mode
             "S"                      ->  vi-substitute-line
             "T"                      ->  vi-charto-back
             "U"                      ->  is undefined
             "V"                      ->  expand-variables
             "W"                      ->  vi-word-fwd
             "X"                      ->  backward-delete-char
             "["                      ->  sequence-lead-in
             "\^"                     ->  beginning-of-line
             "a"                      ->  vi-add
             "b"                      ->  backward-word
             "c"                      ->  is undefined
             "d"                      ->  vi-delmeta
             "e"                      ->  vi-eword
             "f"                      ->  vi-char-fwd
             "h"                      ->  backward-char
             "i"                      ->  vi-insert
             "j"                      ->  down-history
             "k"                      ->  up-history
             "l"                      ->  forward-char
             "m"                      ->  is undefined
             "n"                      ->  vi-search-fwd
             "r"                      ->  vi-replace-char
             "s"                      ->  vi-substitute-char
             "t"                      ->  vi-charto-fwd
             "u"                      ->  vi-undo
             "v"                      ->  expand-variables
             "w"                      ->  vi-beginning-of-next-word
             "x"                      ->  delete-char
             "~"                      ->  change-case
             "^?"                     ->  backward-delete-char
             "M-?"                    ->  run-help
             "M-["                    ->  sequence-lead-in
             "M-O"                    ->  sequence-lead-in

             VI Multi-character bindings

             "^[?"                    ->  run-help

#endif
