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
    CS=Net("SD_CS"),
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
    Vdd=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)


# SUBCIRCUITS
@subcircuit
def cap_combo(tag, val, num, a_net, b_net):
    for i in range(num):
        cap = C_0603(tag=f"{tag}-{i}", value=val)
        a_net & cap & b_net

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


    # Coil connection
    connector["1"] & interface.Vac_a & rectifier["Vac_a"]
    connector["2"] & interface.Vac_b & rectifier["Vac_b"]

    # Rectifier IC
    rectifier["V-"] & interface.GND
    cap_combo("rect_smooth", "22 uF", 5, rectifier["V+"], rectifier["V-"])

    converter_3_3 = Part(
        "tiny_pov_symbols",
        "AP63203WU",
        footprint="tiny_pov_footprints:TSOT-23-6_HandSoldering",
        tag="3v3_converter",
    )

    # See data sheet for typical application circuit of the buck converter
    Net("V_rect_plus") & rectifier["V+"] & converter_3_3["VIN"] & converter_3_3["EN"]
    rectifier["V-"] & converter_3_3["GND"]
    bst_sw_cap = C_0603(value="100 nF", tag="3_3v_bst_sw_cap")
    output_cap = C_0603(value="22 uF", tag="3_3v_output_cap_a") | C_0603(
        value="22 uF", tag="3_3v_output_cap_b"
    )
    output_ind = L_0603(value="6.8uH", tag="3_3v_output_ind")
    Net("BST") & converter_3_3["BST"] & bst_sw_cap[1]
    Net("SW") & converter_3_3["SW"] & bst_sw_cap[2] & output_ind & interface.Vdd
    converter_3_3["FB"] & output_ind[2]
    interface.Vdd & output_cap & interface.GND


# APA102-2020: https://www.mouser.com/datasheet/2/737/APA102_2020_SMD_LED-2487271.pdf
@subcircuit
def apa102_led_strip(num_leds, interface):
    apa102_led = Part(
        "tiny_pov_symbols",
        "APA102-2020",
        dest=TEMPLATE,
        footprint="tiny_pov_footprints:LED_APA-102-2020-256-8",
    )
    # First LED connects to the STM32
    led = apa102_led(tag="apa_led_0")
    led["CKI"] & interface.CLK
    led["SDI"] & interface.DATA

    led["VDD"] += interface.Vdd
    led["GND"] += interface.GND

    # 100 uF cap to prevent brownouts
    cap_combo("surge_cap", "22 uF", 5, led["VDD"], led["GND"])

    # All other LEDs connect to the previous LED
    for i in range(num_leds - 1):
        new_led = apa102_led(tag=f"apa_led_{i + 1}")
        led["CKO"] & Net(f"apa_led_{i}_{i+1}_CK") & new_led["CKI"]
        led["SDO"] & Net(f"apa_led_{i}_{i+1}_SD") &  new_led["SDI"]
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

    # SD card is the only device on the bus, pull CS low
    (
        interface.GND
        & R_0603(value="10k", tag="SD_CS_PULLDOWN")
        & interface.CS
        & sd_holder["CS"]
    )

    # Each data line with a pull up resistor
    (
        sd_holder["SCK"]
        & tee(R_0603(value="47k", tag="SD_SCK_PULLUP") & interface.Vdd)
        & interface.SCK
    )
    (
        sd_holder["MOSI"]
        & tee(R_0603(value="47k", tag="SD_MOSI_PULLUP") & interface.Vdd)
        & interface.MOSI
    )
    (
        sd_holder["MISO"]
        & tee(R_0603(value="47k", tag="SD_MISO_PULLUP") & interface.Vdd)
        & interface.MISO
    )

    # Power and ground
    sd_holder["VDD"] & interface.Vdd
    sd_holder["GND"] & interface.GND & sd_holder[11]


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
    stm = Part(
        "tiny_pov_symbols",
        "STM32G030J6Mx",
        footprint="tiny_pov_footprints:STM32_SO_8N",
        tag="stm",
    )

    # Power
    stm["VDD"] += interface.Vdd
    stm["VSS"] += interface.GND

    # APA102s
    interface.led_clk += stm["p4"] 
    interface.led_data += stm["p8"] 

    # SD card
    interface.sd_sck += stm["p1"]
    interface.sd_miso += stm["p5"]
    interface.sd_mosi += stm["p6"]

    # Hall Effect
    stm["p7"] += interface.hall_data

power_supply(power_supply_interface)
apa102_led_strip(64, apa102_interface)
sd_card(sd_card_interface)
hall_effect(hall_effect_interface)
stm32(stm32_interface)

# EXPORTING
ERC()
generate_netlist()
