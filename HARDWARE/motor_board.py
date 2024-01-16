# Author: Michael Crum
# Date: 11/12/24
# Inductive power supply + motor speed controller.
# Uses a full bridge resonance converter:
# https://www.we-online.com/components/media/o122593v410%20ANP070_EN_Proprietary_wireless_power_transfer_solution_for_high_performance_including_data_transmission.pdf

from skidl import *

set_default_tool(KICAD7)

# TEMPLATES

C_TH = Part(
    "tiny_pov_symbols",
    "C_Polarized",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:C_polarized_TH",
)

# INTERFACES

# SUBCIRCUITS

# 555 Timer

# Full Bridge Driver
# https://fscdn.rohm.com/en/products/databook/datasheet/ic/motor/dc/bd622x-e.pdf

# Motor driver

# EXPORTING
ERC()
generate_netlist()
