[![Build Status](https://travis-ci.org/fabledpig/bspwmtobar.svg?branch=master)](https://travis-ci.org/fabledpig/bspwmtobar)
#bspwmtobar#
Writes and aligns the formatted text written into /tmp/bspwm-fifo, and also formats bspwm's desktop data, which is also expected to be written into /tmp/bspwm-fifo.
```
Syntax: bspwmtobar [-l|-c|-r prefix --color-active color --color-haswindow color --color-normal color]
```

```
-l, -c, -r set alignment to left, center or right
--color-active color of the selected desktop.
--color-haswindow color of desktops that have windows.
--color-normal color of desktops with no windows.
```

```
prefix: a unique identifier that is expected before every line of formatted input to know where to align it
 - the prefix of bspwm's desktop data is 'W' by default
 - expected format: "prefix:"
```

```
color: expected in lemonbar's format #AARRGGBB
```

#Example#
xtitle is written with x:%s to /tmp/bspwm-fifo, and conky has the prefix 'c'
```
bspwmtobar -l x -c W -r c
```

This example uses the default colors, and aligns xtitle's information to the left, desktop data to the center and conky to the right. You can also find a more detailed example in the "example" folder.
