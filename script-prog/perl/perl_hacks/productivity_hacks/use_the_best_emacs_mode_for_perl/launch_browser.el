(global-set-key "\M-p"
  '(lambda () (interactive) (start-process "" nil
  "firefox" "http://localhost:8020/"
  ; Or however you launch your favorite browser, like:
  ;   "gnome-terminal" "-e" "lynx http://localhost:8020/"
  ;   "xterm" "-e" "elinks http://localhost:8020/"
)))
