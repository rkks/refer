(defvar cperl-auto-tidy t
  "Automatically perltidy when saving files.")
(defun cperl-toggle-auto-tidy ()
  "Toggles the variable cperl-auto-tidy."
  (interactive)
  (setq cperl-auto-tidy (not cperl-auto-tidy))
  (message "Automatic perltidy is %s" (if cperl-auto-tidy "on" "off")))
(defun cperl-save-tidy ()
  "Runs perltidy on a buffer and saves it. This is a replacement
   for the normal SAVE-BUFFER function."
  (interactive)
  (and (buffer-modified-p) cperl-auto-tidy
       (save-excursion
         (mark-whole-buffer)
         (perltidy)))
  (save-buffer))
(add-to-list 'cperl-mode-hook
             (lambda ()
               (set (make-local-variable 'cperl-auto-tidy) t)
               (substitute-key-definition
                'save-buffer 'cperl-save-tidy
                cperl-mode-map global-map)))
