# Handle different scalar contexts...
return
        NUM { $elapsed         }
        STR { _HMS($elapsed)   }
  SCALARREF { \ _HMS($elapsed) }
       BOOL { $is_running      }
    HASHREF { {   elapsed => $elapsed,
                  started => $now - $elapsed,
                  running => $is_running,
              }
            }
