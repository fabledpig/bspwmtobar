[![Build Status](https://travis-ci.org/fabledpig/bspwmtobar.svg?branch=master)](https://travis-ci.org/fabledpig/bspwmtobar)
# bspwmtobar
Formatting and printig bspwm's output to [lemonboy's bar](https://github.com/LemonBoy/bar) have never been easier.

## Synopsis
```
bspwmtobar [-l prefix...] [-c prefix...] [-r prefix...] [format_argument format]...
```

```
-l, -c, -r set alignment to left, center or right

prefix: a unique identifier that is expected before every line of formatted input that is written into /tmp/bspwmtobar-fifo (except bspwm's output).
 - the prefix of bspwm' output is 'W' (see Example)
 - expected format: "prefix:%s"
```

bspwmtobar creates /tmp/bspwmtobar-fifo when it starts. That's where you are supposed to redirect the programs' output you want to print to lemonbar. For example:
```
bspc subscribe > /tmp/bspwmtobar-fifo
xtitle -sf "xtitle:%s" > /tmp/bspwmtobar-fifo #notice the "xtitle" prefix
conky > /tmp/bspwmtobar-fifo #conky prints every line starting with "conky:"
```

```
format_argument can be --occupied, --occupied-active, --free, free-active, --urgent, --urgent-active
 - format_arguments resemble desktop states defined by bspwm

format is expected in lemonbar's formatting style (only the opening of the formatting syntax, closing is done automatically, for example %{F-} etc.).

Make sure to define a format for --free and --free-active at least.
```

## Installation
```
sudo make install
```

## Example
xtitle is written with prefix xtitle, and conky has the prefix conky.
```
bspwmtobar -l xtitle -c W -r conky
```
This example aligns xtitle's information to the left, bspwm's output to the center and conky to the right.
Make sure to checkout the example folder, it contains a more detailed example.
