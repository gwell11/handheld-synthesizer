# DaisySeedGFX
TFT graphical display library for Daisy Seed. ST7789 driver

## Author
DAD Design

## Présentation
DaisySeedGFX is a graphical library for the Daisy Seed platform by Electrosmith.
The library currently only supports the ST7789 controller. I plan to quickly integrate the ST7735 controller. In principle, the library should be easily adaptable to any controller.

The library uses an intermediate frame buffer between the graphic primitives and the controller. Modifications made in the frame buffer are transmitted to the controller using SPI transfers under DMA. To reduce transfers, the frame buffer is divided into blocks -> only the modified blocks are transferred to the screen.

The graphic primitives are minimalist and can be completed according to needs.

## Configuration
The UserConfig.h file allows adapting the library to different hardware implementations.

The Font.h file is used to define the loaded fonts. Each font is described in a xxx.h file, which can be created using the tool https://rop.nl/truetype2gfx/.

## Development Tool
The code can be modified and compiled under VS Code in the Daisy Seed development environment (see https://github.com/electro-smith).

The Makefile must be adapted according to the location of the libraries:

LIBDAISY_DIR = ../../../Desktop/DaisyExamples/libDaisy/
DAISYSP_DIR = ../../../Desktop/DaisyExamples/DaisySP/


The c_cpp_properties.json file must also be adapted:

"includePath": [
  "${workspaceFolder}/**",
  "${workspaceFolder}/../../../Desktop/DaisyExamples/libDaisy/**",
  "${workspaceFolder}/../../../Desktop/DaisyExamples/DaisySP/**"
]
