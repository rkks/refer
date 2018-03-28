(require 'pod-mode)
(add-to-list 'auto-mode-alist
  '("\\.pod$" . pod-mode))

; You might appreciate turning on these
;   features by default for Pod:

(add-hook 'pod-mode-hook '(lambda () (progn
 (font-lock-mode)   ; =syntax highlighting
 (auto-fill-mode 1) ; =wordwrap
 (flyspell-mode 1)  ; =spellchecking
)))
