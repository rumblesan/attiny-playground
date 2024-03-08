# Firmware Programming

## MacOS

The easiest way to get this all setup on MacOS is to get [Homebrew](https://brew.sh/) setup, and then install avr-gcc and avrdude through that.

You'll need to install the osx-cross brew tap first though from <https://github.com/osx-cross/homebrew-avr>.

```
brew install avr-gcc avrdude
```

Make should already be on your system, so just open up a terminal, navigate to the firmware directory, and then run `make install`. If it's the first time you've programmed the chip then you'll probably have to run `make fuses` as well to set the correct clock speed on the chip.

## Windows

This almost certainly needs some work to sort out better.
<https://tinusaur.com/guides/avr-gcc-toolchain/> has some info but it's untried.
