from skidl import *

set_default_tool(KICAD7)

# TEMPLATES

apa102_led = Part("tiny_pov_symbols", "APA102-2020", dest=TEMPLATE)

# INTERFACES

power_supply_interface = Interface(
    Vac_a=Net("Vac_a"), Vac_b=Net("Vac_b"), Vdd=Net("Vdd"), GND=Net("GND")
)

apa102_interface = Interface(
    clk=Net("LED_CLK"),
    data=Net("LED_DATA"),
    Vdd=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)


stm32_interface = Interface(
    VDD=power_supply_interface.Vdd,
    GND=power_supply_interface.GND,
)

# SUBCIRCUITS


@subcircuit
def power_supply(interface):
    pass


@subcircuit
def stm32_minimal(interface):
    pass


@subcircuit
def apa102_led_strip(num_leds, interface):
    led = apa102_led()
    led["CKI"] += interface.clk
    led["SDI"] += interface.data
    led["GND"] += gnd
    led["VDD"] += pwr_3_3v
    for _ in range(num_leds):
        new_led = apa102_led()
        led["CKO"] += new_led["CKI"]
        led["SDO"] += new_led["SDI"]
        new_led["GND"] += gnd
        new_led["VDD"] += pwr_3_3v
        new_led = new_led


# EXPORTING

apa102_led_strip(10, apa102_interface)
ERC()
