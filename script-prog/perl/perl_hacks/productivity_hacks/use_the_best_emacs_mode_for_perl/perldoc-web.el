(global-set-key "\M-p" '(lambda () (interactive)
  (require 'w3m)
  (w3m-goto-url "http://localhost:8020/")
))
