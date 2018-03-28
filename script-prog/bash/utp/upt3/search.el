;
; Repeatable non-incremental search.
;
;    ^S - Search forward.  You will be prompted for the string to search for.
;    ^R - Search backward. You will be pormpted for the string to search for.
;    ^S^S - Search forward for last requested string.
;    ^R^R - Search backward for last requested string.
;
; Written for Mike loukides.
;

;
; Author: Chris Hind Genly 10/3/88
;


(define-key global-map "\C-r" 'msearch-backward)
(define-key global-map "\C-s" 'msearch-forward)

(defun msearch-forward ()
  (interactive)
  (msearch t)
)

(defun msearch-backward ()
  (interactive)
  (msearch nil)
)

(setq msearch-string "")

(defun msearch (forward)
  (interactive)
  (if forward
      (message (concat "M-Search [" msearch-string "]: "))
      (message (concat "M-Search-backward [" msearch-string "]: "))
      )
  (setq c (read-char))
  (cond
   ((eq c ?\C-s) (search-forward msearch-string))
   ((eq c ?\C-r) (search-backward msearch-string))
   (t (progn
        (setq msearch-string (read-string
                              (if forward
                                  "M-Search: "
                                  "M-Search-backward: ")
                              (char-to-string c)))
        (if forward
            (search-forward msearch-string)
            (search-backward msearch-string)
            )
        )
      )
   ))

