from skidl import *

set_default_tool(KICAD7)

# INTERFACES

power_supply_interface = Interface(
    Vac_a=Net("Vac_a"), Vac_b=Net("Vac_b"), Vdd=Net("Vdd"), GND=Net("GND")
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
    RESET=hall_effect_interface.DATA,
    Vdd=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)


# SUBCIRCUITS


@subcircuit
def power_supply(interface):
    pass


@subcircuit
def apa102_led_strip(num_leds, interface):
    apa102_led = Part(
        "tiny_pov_symbols",
        "APA102-2020",
        dest=TEMPLATE,
        footprint="tiny_pov_footprints.pretty:LED-APA102-2020",
    )
    # First LED connects to the STM32
    led = apa102_led()
    led["CKI"] += interface.CLK
    led["SDI"] += interface.DATA

    led["VDD"] += interface.Vdd
    led["GND"] += interface.GND

    # 100 uF cap to prevent brownouts
    surge_cap = Part(
        "tiny_pov_symbols",
        "C",
        value="100 uF",
        footprint="tiny_pov_footprints.pretty:C_0402_hand_solder",
    )
    led["VDD"] & surge_cap & led["GND"]

    # All other LEDs connect to the previous LED
    for _ in range(num_leds):
        new_led = apa102_led()
        led["CKO"] += new_led["CKI"]
        led["SDO"] += new_led["SDI"]
        new_led["VDD"] += interface.Vdd
        new_led["GND"] += interface.GND
        new_led = new_led


@subcircuit
def sd_card(interface):
    pass


@subcircuit
def hall_effect(interface):
    hall = Part(
        "tiny_pov_symbols",
        "AH1806W",
        footprint="tiny_pov_footprints.pretty:SC-59_Handsoldering",
    )
    hall["VDD"] += interface.Vdd
    hall["GND"] += interface.GND
    hall["OUTPUT"] += interface.DATA


# https://www.st.com/resource/en/datasheet/stm32g030c6.pdf
@subcircuit
def stm32(interface):
    # Power
    stm = Part(
        "tiny_pov_symbols",
        "STM32G030J6Mx",
        footprint="tiny_pov_footprints.pretty:STM32_SO_8N",
    )
    stm["VDD"] += interface.Vdd
    stm["VSS"] += interface.GND

    stm["p4"].aliases += "NRST"

    # Reset will be used as GPIO
    # Upon power cycle, reset gets mapped back to NRST.
    # When jumper is bridged, to ground it will remain in that state and allow for programming.
    stm["NRST"] & interface.RESET
    # Active low, placed in parallel with a 100nF cap as mentioned on page 66 of the data sheet
    reset_jumper = Part(
        "tiny_pov_symbols",
        "SolderJumper_2_Open",
        footprint="tiny_pov_footprints.pretty:SolderJumper",
    )
    reset_cap = Part(
        "tiny_pov_symbols",
        "C",
        value="100 nF",
        footprint="tiny_pov_footprints.pretty:C_0402_hand_solder",
    )
    stm["NRST"] & reset_jumper & interface.GND
    stm["NRST"] & reset_cap & interface.GND  # place close to chip


# EXPORTING

power_supply(power_supply_interface)
apa102_led_strip(10, apa102_interface)
sd_card(sd_card_interface)
hall_effect(hall_effect_interface)
stm32(stm32_interface)

ERC()
generate_netlist()
