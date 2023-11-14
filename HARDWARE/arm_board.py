# Author: Michael Crum
# Date: 11/14/23
# Circuit description for Tiny POV's arm circuit board. This is the part that rotates.
# Inductively powered from the board located in motor_board.py


from skidl import *

set_default_tool(KICAD7)

# TEMPLATES
jumper = Part(
    "tiny_pov_symbols",
    "SolderJumper_2_Open",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:SolderJumper",
)

test_point_pair = Part(
    "tiny_pov_symbols",
    "TestPoint_2Pole",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:PinHeader_1x02",
)

test_point = Part(
    "tiny_pov_symbols",
    "TestPoint",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:TestPoint_Pad",
)

R_0402 = Part(
    "tiny_pov_symbols",
    "R",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:R_0402_hand_solder",
)

C_0402 = Part(
    "tiny_pov_symbols",
    "C",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:C_0603_hand_solder",
)

R_0603 = Part(
    "tiny_pov_symbols",
    "R",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:R_0603_hand_solder",
)

C_0603 = Part(
    "tiny_pov_symbols",
    "C",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:C_0603_hand_solder",
)

C_TH = Part(
    "tiny_pov_symbols",
    "C_Polarized",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:C_polarized_TH",
)

L_0603 = Part(
    "tiny_pov_symbols",
    "L",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:L_0603_hand_solder",
)


# INTERFACES

power_supply_interface = Interface(
    Vac_a=Net("Vac_a", drive=POWER),
    Vac_b=Net("Vac_b", drive=POWER),
    Vdd=Net("Vdd", drive=POWER),
    GND=Net("GND", drive=POWER),
)

apa102_interface = Interface(
    CLK=Net("LED_CLK"),
    DATA=Net("LED_DATA"),
    Vdd=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)

sd_card_interface = Interface(
    SCK=Net("SD_SCK"),
    MOSI=Net("SD_MOSI"),
    MISO=Net("SD_MISO"),
    CS=Net(),
    Vdd=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)

hall_effect_interface = Interface(
    DATA=Net("HALL_DATA"),
    Vdd=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)

stm32_interface = Interface(
    led_clk=apa102_interface.CLK,
    led_data=apa102_interface.DATA,
    sd_sck=sd_card_interface.SCK,
    sd_mosi=sd_card_interface.MOSI,
    sd_miso=sd_card_interface.MISO,
    hall_data=hall_effect_interface.DATA,
    RESET=Net(),
    Vdd=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)


# SUBCIRCUITS


# 3.3v converter: https://www.diodes.com/assets/Datasheets/AP63200-AP63201-AP63203-AP63205.pdf
# Full bridge rectifier: https://www.diodes.com/assets/Datasheets/DBF310.pdf
@subcircuit
def power_supply(interface):
    connector = Part(
        "tiny_pov_symbols",
        "Conn_01x02_Socket",
        footprint="tiny_pov_footprints:PinHeader_1x02",
        tag="pwr_supply",
    )

    rectifier = Part(
        "tiny_pov_symbols",
        "DBF310-13",
        footprint="tiny_pov_footprints:DBF_DIO",
        tag="rectifier",
    )

    smoothing_cap = C_TH(tag="smoothing_cap", value="? uF")

    # Coil connection
    connector["1"] & interface.Vac_a & rectifier["Vac_a"]
    connector["2"] & interface.Vac_b & rectifier["Vac_b"]

    # Rectifier IC
    rectifier["V+"] & smoothing_cap[1]
    rectifier["V-"] & smoothing_cap[2] & interface.GND

    converter_3_3 = Part(
        "tiny_pov_symbols",
        "AP63203WU",
        footprint="tiny_pov_footprints:TSOT-23-6_HandSoldering",
        tag="3v3_converter",
    )

    # See data sheet for typical application circuit of the buck converter
    rectifier["V+"] & converter_3_3["VIN"] & converter_3_3["EN"]
    rectifier["V-"] & converter_3_3["GND"]
    bst_sw_cap = C_0603(value="100 nF", tag="3_3v_bst_sw_cap")
    output_cap = C_0603(value="22 uF", tag="3_3v_output_cap_a") | C_0603(
        value="22 uF", tag="3_3v_output_cap_b"
    )
    output_ind = L_0603(value="6.8uH", tag="3_3v_output_ind")
    converter_3_3["BST"] & bst_sw_cap[1]
    converter_3_3["SW"] & bst_sw_cap[2] & output_ind & interface.Vdd
    converter_3_3["FB"] += output_ind[2]
    interface.Vdd & output_cap & interface.GND

    # v_dd_test = test_point(tag="v_dd_test")
    # v_gnd_test = test_point(tag="v_gnd_test")
    # interface.Vdd += v_dd_test
    # interface.GND += v_gnd_test


# APA102-2020: https://www.mouser.com/datasheet/2/737/APA102_2020_SMD_LED-2487271.pdf
@subcircuit
def apa102_led_strip(num_leds, interface):
    apa102_led = Part(
        "tiny_pov_symbols",
        "APA102-2020",
        dest=TEMPLATE,
        footprint="tiny_pov_footprints:LED-APA102-2020",
    )
    # First LED connects to the STM32
    led = apa102_led(tag="apa_led_0")
    led_clk_test = test_point(tag="led_clk_test")
    led_data_test = test_point(tag="led_data_test")
    led["CKI"] & led_clk_test & interface.CLK
    led["SDI"] & led_data_test & interface.DATA

    led["VDD"] += interface.Vdd
    led["GND"] += interface.GND

    # 100 uF cap to prevent brownouts
    surge_cap = C_TH(value="100 uF", tag="surge_cap")
    led["VDD"] & surge_cap & led["GND"]

    # All other LEDs connect to the previous LED
    for i in range(num_leds - 1):
        new_led = apa102_led(tag=f"apa_led_{i + 1}")
        led["CKO"] += new_led["CKI"]
        led["SDO"] += new_led["SDI"]
        new_led["VDD"] += interface.Vdd
        new_led["GND"] += interface.GND
        led = new_led

    led["CKO"] += NC
    led["SDO"] += NC


# SD Card Holder: https://tools.molex.com/pdm_docs/sd/5033981892_sd.pdf
@subcircuit
def sd_card(interface):
    sd_holder = Part(
        "tiny_pov_symbols",
        "SdHolder",
        footprint="tiny_pov_footprints:microSD",
        tag="sd_holder",
    )

    # SD card is the only device on the bus, pull it low
    (
        interface.GND
        & R_0603(value="10k", tag="SD_CS_PULLDOWN")
        & interface.CS
        & sd_holder["CS"]
    )

    sd_sck_test = test_point(tag="sd_sck_test_point")
    sd_mosi_test = test_point(tag="sd_mositest_point")
    sd_miso_test = test_point(tag="sd_miso_test_point")

    # Each data line with a pull up resistor
    (
        sd_holder["SCK"]
        & sd_sck_test
        & R_0603(value="47k", tag="SD_SCK_PULLUP")
        & interface.SCK
    )
    (
        sd_holder["MOSI"]
        & sd_mosi_test
        & R_0603(value="47k", tag="SD_MOSI_PULLUP")
        & interface.MOSI
    )
    (
        sd_holder["MISO"]
        & sd_miso_test
        & R_0603(value="47k", tag="SD_MISO_PULLUP")
        & interface.MISO
    )

    # Power and ground
    sd_holder["GND"] & interface.GND
    sd_holder["VDD"] & interface.Vdd


# Hall effect sensor: https://www.diodes.com/assets/Datasheets/AH1806.pdf
@subcircuit
def hall_effect(interface):
    hall = Part(
        "tiny_pov_symbols",
        "AH1806W",
        footprint="tiny_pov_footprints:SC-59_Handsoldering",
        tag="hall_effect",
    )
    hall["VDD"] += interface.Vdd
    hall["GND"] += interface.GND
    hall["OUTPUT"] += interface.DATA


# STM32G0 (SO8N package): https://www.st.com/resource/en/datasheet/stm32g030c6.pdf
@subcircuit
def stm32(interface):
    # Power
    stm = Part(
        "tiny_pov_symbols",
        "STM32G030J6Mx",
        footprint="tiny_pov_footprints:STM32_SO_8N",
        tag="stm",
    )
    stm["VDD"] += interface.Vdd
    stm["VSS"] += interface.GND

    stm["p4"].aliases += "NRST"

    # Reset will be used as GPIO
    # Upon power cycle, reset gets mapped back to NRST.
    # When jumper is bridged it will remain in that state and allow for programming.
    stm["NRST"] & interface.RESET
    # Active low, placed in parallel with a 100nF cap as mentioned on page 66 of the data sheet
    reset_jumper = jumper(tag="reset_jumper")
    reset_cap = C_0402(
        value="100 nF",
        tag="reset_cap",
    )
    stm["NRST"] & reset_jumper & interface.GND
    stm["NRST"] & reset_cap & interface.GND  # place close to chip

    # APA102s
    stm["p7"] += interface.led_clk
    stm["p6"] += interface.led_data

    # SD card
    stm["p1"] += interface.sd_sck
    stm["p8"] += interface.sd_miso
    stm["p5"] += interface.sd_mosi

    # Hall Effect
    stm["NRST"] += interface.hall_data  # :|


# EXPORTING

power_supply(power_supply_interface)
apa102_led_strip(64, apa102_interface)
sd_card(sd_card_interface)
hall_effect(hall_effect_interface)
stm32(stm32_interface)

ERC()
generate_netlist()
