# Hardware

Tiny-pov was designed using [SKiDL](https://devbisme.github.io/skidl/), a python library for generating KiCAD schematics from python code. 
The SKiDL source code for the PCB is found in `board.py`. The KiCAD symbol and schematic libraries for all components is included, so it should run without any hassle.

```cd HARDWARE && python3 board.py```

For power delivery, I designed a custom 3D printed inductive power coupling inspired by electric toothbrushes. It uses a core [ferrous 3D printer filament](https://proto-pasta.com/products/magnetic-iron-pla) to increase the efficiency of transformer.
You can find the design files on [OnShape](https://cad.onshape.com/documents/c43b3a736df7726a49e53755/w/ad83221bbc41dda85aa40265/e/0c03297652a8e1a4c52ec09d?renderMode=0&uiState=655283aa916fc55b212fd806).
