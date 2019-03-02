# Sweet Leaf firmware

## Directory structure

It's a mess, because I wanted the project to be regenerate'able from CubeMX.

- `Core` and `Drivers` directories contain Cube-specific files. Who the hell
  uses uppercase in folders anyway? Most of those files are git-ignored.
- True project files reside in `sweet_leaf` directory.

## Build instructions

CubeMX generates Makefile but it can't be reliably edited when regenerating
project. This project uses tupfile instead.

Prerequisites:

- ARM GCC
- STM32 CubeMX (needs Java RE)
- [tup](http://gittup.org/tup/), type `tup init` inside this directory

Launch `firmware.ioc` and generate project files in current directory. Then
type `tup` to (re)build the project.

### Pin assignments for Nucleo F722ZE

See UM1974 for more details - table 14., page 43:

Inner pins:

   ``` plain
                                       PC6   1 2   PB8
                                      PB15   3 4   PB9
       NC  1 2   PC8                  PB13   5 6   AREF
    IOREF  3 4   PC9                  PB12   7 8   GND
     NRST  5 6   PC1                  PA15   9 10  PA5
      3V3  7 8   PC1                   PC7  11 12  PA6
       5V  9 10  PC1                   PB5  13 14  PA7/PB5
      GND 11 12  PD2                   PB3  15 16  PD14
      GND 13 14  PG2                   PA4  17 18  PD15
      VIN 15 16  PG3                   PB4  19 20  PF12
           CN8                               CN7
           CN9                               CN10
     PA3   1 2   PD7                  AVDD   1 2   PF13
     PC0   3 4   PD6                  AGND   3 4   PE9
     PC3   5 6   PD5        uart_GND - GND   5 6   PE11
     PF3   7 8   PD4                   PB1   7 8   PF14
PF5/PB10   9 10  PD3                   PC2   9 10  PE13
PF10/PB8  11 12  GND                   PF4  11 12  PF15
      NC  13 14  PE2                   PB6  13 14  PG14
      NC  15 16  PE4                   PB2  15 16  PG9
     PF2  17 18  PE5                   GND  17 18  PE8
     PF1  19 20  PE6                  PD13  19 20  PE7
     PF0  21 22  PE3            A17 - PD12  21 22  GND
     GND  23 24  PF8                  PD11  23 24  PE10 - D7
     PD0  25 26  PF7                   PE2  25 26  PE12
     PD1  27 28  PF9                   GND  27 28  PE14
     PG0  29 30  PG1                   PA0  29 30  PE15
                                       PB0  31 32  PB10 - uart_RX
                                       PE0  33 34  PB11 - uart_TX

```

Outer pins (morpho connector) - table 21., page 70:

``` plain
               CN11                            CN12
 LSEN - PC10   1 2   PC11                PC9   1 2   PC8
        PC12   3 4   PD2                 PB8   3 4   PC6
         VDD   5 6   E5V                 PB9   5 6   PC5
        BOOT   7 8   GND                AVDD   7 8   U5V
         PF6   9 10  NC                  GND   9 10  PD8  - D13
         PF7  11 12  IOREF               PA5  11 12  PA12
        PA13  13 14  RES                 PA6  13 14  PA11
        PA14  15 16  3V3                 PA7  15 16  PB12
        PA15  17 18  5V                  PB6  17 18  PB11
         GND  19 20  GND                 PC7  19 20  GND
   NL -  PB7  21 22  GND                 PA9  21 22  PB2
        PC13  23 24  VIN                 PA8  23 24  PB1
        PC14  25 26  NC                 PB10  25 26  PB15
        PC15  27 28  PA0                 PB4  27 28  PB14
  CLK -  PH0  29 30  PA1                 PB5  29 30  PB13
         PH1  31 32  PA4                 PB3  31 32  AGND
        VBAT  33 34  PB0                PA10  33 34  PC4
         PC2  35 36  PC1                 PA2  35 36  PF5  - A5
         PC3  37 38  PC0                 PA3  37 38  PF4  - A4
  NOE -  PD4  39 40  PD3                 GND  39 40  PE8  - D5
  NWE -  PD5  41 42  PG2  - A12   A18 - PD13  41 42  PF10
NWAIT -  PD6  43 44  PG3  - A13         PA12  43 44  PE7  - D4
  NE1 -  PD7  45 46  PE2  - A23   A16 - PD11  45 46  PD14 - D0
  A19 -  PE3  47 48  PE4  - A20         PE10  47 48  PD15 - D1
         GND  49 50  PE5  - A21    D9 - PE12  49 50  PF14 - A8
   A1 -  PF1  51 52  PF2  - A2    D11 - PE14  51 52  PE9  - D6
   A0 -  PF0  53 54  PF8          D12 - PE15  53 54  GND
   D3 -  PD1  55 56  PF9          D10 - PE13  55 56  PE11 - D8
   D2 -  PD0  57 58  PG1  - A11    A7 - PF13  57 58  PF3  - A3
  A10 -  PG0  59 60  GND           A6 - PF12  59 60  PF15 - A9
 NBL1 -  PE1  61 62  PE6  - A22         PG14  61 62  PF11
         PG9  63 64  PG15                GND  63 64  PE0  - NBL0
        PG12  65 66  PG10         D15 - PD10  65 66  PG8
          NC  67 68  PG13                PG7  67 68  PG5  - A15
  D14 -  PD9  69 70  PG11         A14 -  PG4  69 70  PG6
```

Pins:

- D0-D15 - data bus
- A0 - A23 - address bus
- CLK - external clock in - don't level shift, connect directly
- LSEN - Level Shifter Enable

## License

Unless otherwise stated, the sources are licensed under MPL 2.0.
See LICENSE for details.

TLDR: if you base your own project on MPL sources, you need to publish your
changes to them.

## 3rd party stuff used

I'm not keeping those as submodules to keep project tree simple. They'll get
updated manually if there's need or something.

- [printf / sprintf for embedded systems](https://github.com/mpaland/printf)
