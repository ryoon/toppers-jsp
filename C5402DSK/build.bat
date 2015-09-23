cl /E /EP /I "../kernel" /I "../include"  /I "../config/tms320c54x" /I "../config/tms320c54x/c5402dsk" sample1.cfg >sample1_i.cfg
..\cfg\cfg.exe -s sample1_i.cfg  -v -obj --ZERO "x y[1]"