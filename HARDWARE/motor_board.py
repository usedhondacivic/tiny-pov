# Author: Michael Crum
# Date: 11/14/23
# Circuit description for Tiny POV's motor and power supply board.
# Regulates motor speed + uses a self regulating flyback converter to transfer power to the arm.
# Self reference: https://electronics.stackexchange.com/questions/506874/simple-self-oscillating-flyback-converter


from skidl import *

set_default_tool(KICAD7)
