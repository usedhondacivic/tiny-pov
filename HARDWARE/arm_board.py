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
    footprint="tiny_pov_footprints:TestPoint_2Point",
)

R_0402 = Part(
    "tiny_pov_symbols",
    "R",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:R_0402_hand_solder",
)

C_0402 = Part(
    "tiny_pov_symbols",
    "C_Polarized",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:C_polarized_TH",
)

C_TH = Part(
    "tiny_pov_symbols",
    "C_Polarized",
    dest=TEMPLATE,
    footprint="tiny_pov_footprints:C_polarized_TH",
)


# INTERFACES

power_supply_interface = Interface(
    Vac_a=Net("Vac_a", drive=POWER),
    Vac_b=Net("Vac_b", drive=POWER),
    Vdd=Net("Vdd"),
    GND=Net("GND"),
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


@subcircuit
def power_supply(interface):
    # add resonant cap for inductor
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

    test_pair = test_point_pair(tag="rectifier_test")
    # Rectifier IC
    rectifier["V+"] & smoothing_cap[1] & test_pair[1] & interface.Vdd
    rectifier["V-"] & smoothing_cap[2] & test_pair[2] & interface.GND


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
    test_pair = test_point_pair(tag="APA102_test")
    led["CKI"] & test_pair[1] & interface.CLK
    led["SDI"] & test_pair[2] & interface.DATA

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
        & R_0402(value="10k", tag="SD_CS_PULLDOWN")
        & interface.CS
        & sd_holder["CS"]
    )

    clock_test = test_point_pair(tag="sd_clock_test_points")
    data_test = test_point_pair(tag="sd_data_test_points")

    interface.GND & clock_test[1]
    # Each data line with a pull up resistor
    (
        sd_holder["SCK"]
        & clock_test[2]
        & R_0402(value="47k", tag="SD_SCK_PULLUP")
        & interface.SCK
    )
    (
        sd_holder["MOSI"]
        & data_test[1]
        & R_0402(value="47k", tag="SD_MOSI_PULLUP")
        & interface.MOSI
    )
    (
        sd_holder["MISO"]
        & data_test[2]
        & R_0402(value="47k", tag="SD_MISO_PULLUP")
        & interface.MISO
    )

    # Power and ground
    sd_holder["GND"] & interface.GND
    sd_holder["VDD"] & interface.Vdd


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


# https://www.st.com/resource/en/datasheet/stm32g030c6.pdf
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
    reset_jumper = test_point_pair(tag="reset_jumper")
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
apa102_led_strip(10, apa102_interface)
sd_card(sd_card_interface)
hall_effect(hall_effect_interface)
stm32(stm32_interface)

ERC()
generate_netlist()
