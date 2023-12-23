from skidl import Pin, Part, Alias, SchLib, SKIDL, TEMPLATE

SKIDL_lib_version = '0.0.1'

arm_board_lib = SchLib(tool=SKIDL).add_parts(*[
        Part(**{ 'name':'Conn_01x02_Socket', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'Conn_01x02_Socket'}), '_name':'Conn_01x02_Socket', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'connector', 'description':'Generic connector, single row, 01x02, script generated', 'ref_prefix':'J', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'Conn_01x02_Socket'}), 'pin':None, 'footprint':'tiny_pov_footprints:PinHeader_1x02', 'pins':[
            Pin(num='1',name='Pin_1',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='Pin_2',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'DBF310-13', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'DBF310-13'}), '_name':'DBF310-13', '_match_pin_regex':False, 'datasheet':'DBF310-13', 'keywords':'DBF310-13', 'description':'', 'ref_prefix':'BR', 'num_units':None, 'fplist':['DBF_DIO'], 'do_erc':True, 'aliases':Alias({'DBF310-13'}), 'pin':None, 'footprint':'tiny_pov_footprints:DBF_DIO', 'pins':[
            Pin(num='1',name='V+',func=Pin.types.PWROUT,do_erc=True),
            Pin(num='2',name='Vac_b',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='3',name='V-',func=Pin.types.PWROUT,do_erc=True),
            Pin(num='4',name='Vac_a',func=Pin.types.PWRIN,do_erc=True)] }),
        Part(**{ 'name':'C_Polarized', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'C_Polarized'}), '_name':'C_Polarized', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'cap capacitor', 'description':'Polarized capacitor', 'ref_prefix':'C', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'C_Polarized'}), 'pin':None, 'footprint':'tiny_pov_footprints:C_polarized_TH', 'pins':[
            Pin(num='1',name='~',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='~',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'AP63203WU', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'AP63203WU'}), '_name':'AP63203WU', '_match_pin_regex':False, 'datasheet':'https://www.diodes.com/assets/Datasheets/AP63200-AP63201-AP63203-AP63205.pdf', 'keywords':'2A Buck DC/DC', 'description':'2A, 1.1MHz Buck DC/DC Converter, fixed 3.3V output voltage, TSOT-23-6', 'ref_prefix':'U', 'num_units':None, 'fplist':['Package_TO_SOT_SMD:TSOT-23-6'], 'do_erc':True, 'aliases':Alias({'AP63203WU'}), 'pin':None, 'footprint':'tiny_pov_footprints:TSOT-23-6_HandSoldering', 'pins':[
            Pin(num='1',name='FB',func=Pin.types.INPUT,do_erc=True),
            Pin(num='2',name='EN',func=Pin.types.INPUT,do_erc=True),
            Pin(num='3',name='VIN',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='4',name='GND',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='5',name='SW',func=Pin.types.OUTPUT,do_erc=True),
            Pin(num='6',name='BST',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'C', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'C'}), '_name':'C', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'cap capacitor', 'description':'Unpolarized capacitor', 'ref_prefix':'C', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'C'}), 'pin':None, 'footprint':'tiny_pov_footprints:C_0603_hand_solder', 'pins':[
            Pin(num='1',name='~',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='~',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'L', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'L'}), '_name':'L', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'inductor choke coil reactor magnetic', 'description':'Inductor', 'ref_prefix':'L', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'L'}), 'pin':None, 'footprint':'tiny_pov_footprints:L_0603_hand_solder', 'pins':[
            Pin(num='1',name='1',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='2',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'APA102-2020', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'APA102-2020'}), '_name':'APA102-2020', '_match_pin_regex':False, 'datasheet':'http://www.led-color.com/upload/201604/APA102-2020%20SMD%20LED.pdf', 'keywords':'RGB LED addressable 8bit pwm 5bit greyscale', 'description':'RGB LED with integrated controller', 'ref_prefix':'D', 'num_units':None, 'fplist':['LED_SMD:LED-APA102-2020'], 'do_erc':True, 'aliases':Alias({'APA102-2020'}), 'pin':None, 'footprint':'tiny_pov_footprints:LED-APA102-2020', 'pins':[
            Pin(num='1',name='VDD',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='2',name='CKO',func=Pin.types.OUTPUT,do_erc=True),
            Pin(num='3',name='SDO',func=Pin.types.OUTPUT,do_erc=True),
            Pin(num='4',name='SDI',func=Pin.types.INPUT,do_erc=True),
            Pin(num='5',name='CKI',func=Pin.types.INPUT,do_erc=True),
            Pin(num='6',name='GND',func=Pin.types.PWRIN,do_erc=True)] }),
        Part(**{ 'name':'TestPoint', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'TestPoint'}), '_name':'TestPoint', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'test point tp', 'description':'test point', 'ref_prefix':'TP', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'TestPoint'}), 'pin':None, 'footprint':'tiny_pov_footprints:TestPoint_Pad', 'pins':[
            Pin(num='1',name='1',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'SdHolder', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'SdHolder'}), '_name':'SdHolder', '_match_pin_regex':False, 'datasheet':'1040310811', 'keywords':'1040310811', 'description':'', 'ref_prefix':'J', 'num_units':None, 'fplist':['104031-0811_MOL'], 'do_erc':True, 'aliases':Alias({'SdHolder'}), 'pin':None, 'footprint':'tiny_pov_footprints:microSD', 'pins':[
            Pin(num='4',name='VDD',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='1',name='DAT2',func=Pin.types.NOCONNECT,do_erc=True),
            Pin(num='10',name='SW_DETECT',func=Pin.types.NOCONNECT,do_erc=True),
            Pin(num='2',name='CS',func=Pin.types.INPUT,do_erc=True),
            Pin(num='3',name='MOSI',func=Pin.types.INPUT,do_erc=True),
            Pin(num='5',name='SCK',func=Pin.types.INPUT,do_erc=True),
            Pin(num='6',name='GND',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='7',name='MISO',func=Pin.types.OUTPUT,do_erc=True),
            Pin(num='8',name='DAT1',func=Pin.types.NOCONNECT,do_erc=True),
            Pin(num='9',name='SW_DETECT',func=Pin.types.NOCONNECT,do_erc=True)] }),
        Part(**{ 'name':'R', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'R'}), '_name':'R', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'R res resistor', 'description':'Resistor', 'ref_prefix':'R', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'R'}), 'pin':None, 'footprint':'tiny_pov_footprints:R_0603_hand_solder', 'pins':[
            Pin(num='1',name='~',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='~',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'AH1806W', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'AH1806W'}), '_name':'AH1806W', '_match_pin_regex':False, 'datasheet':'https://www.diodes.com/assets/Datasheets/AH1806.pdf', 'keywords':'hall switch', 'description':'High Sensitivity Micropower Omnipolar Hall-Effect Switch, SC-59', 'ref_prefix':'U', 'num_units':None, 'fplist':['Package_TO_SOT_SMD:SC-59'], 'do_erc':True, 'aliases':Alias({'AH1806W'}), 'pin':None, 'footprint':'tiny_pov_footprints:SC-59_Handsoldering', 'pins':[
            Pin(num='1',name='VDD',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='2',name='OUTPUT',func=Pin.types.OPENCOLL,do_erc=True),
            Pin(num='3',name='GND',func=Pin.types.PWRIN,do_erc=True)] }),
        Part(**{ 'name':'STM32G030J6Mx', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'STM32G030J6Mx'}), '_name':'STM32G030J6Mx', '_match_pin_regex':False, 'datasheet':'https://www.st.com/resource/en/datasheet/stm32g030j6.pdf', 'keywords':'Arm Cortex-M0+ STM32G0 STM32G0x0 Value line', 'description':'STMicroelectronics Arm Cortex-M0+ MCU, 32KB flash, 8KB RAM, 64 MHz, 2.0-3.6V, 6 GPIO, SO8N', 'ref_prefix':'U', 'num_units':None, 'fplist':['Package_SO:SOIC-8_3.9x4.9mm_P1.27mm'], 'do_erc':True, 'aliases':Alias({'STM32G030J6Mx'}), 'pin':None, 'footprint':'tiny_pov_footprints:STM32_SO_8N', 'pins':[
            Pin(num='1',name='PC14/PB7/PB8/PB9',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='2',name='VDD',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='3',name='VSS',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='4',name='NRST/PA0/PA1/PA2',func=Pin.types.INPUT,do_erc=True),
            Pin(num='5',name='PB0/PB1/PA8/PA11/PA9',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='6',name='PA12/PA10',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='7',name='PA13',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='8',name='PA14/PA15/PB5/PB6',func=Pin.types.BIDIR,do_erc=True)] }),
        Part(**{ 'name':'SolderJumper_2_Open', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'SolderJumper_2_Open'}), '_name':'SolderJumper_2_Open', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'solder jumper SPST', 'description':'Solder Jumper, 2-pole, open', 'ref_prefix':'JP', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'SolderJumper_2_Open'}), 'pin':None, 'footprint':'tiny_pov_footprints:SolderJumper', 'pins':[
            Pin(num='1',name='A',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='B',func=Pin.types.PASSIVE,do_erc=True)] })])