from skidl import Pin, Part, Alias, SchLib, SKIDL, TEMPLATE

SKIDL_lib_version = '0.0.1'

board_lib = SchLib(tool=SKIDL).add_parts(*[
        Part(**{ 'name':'APA102-2020', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'APA102-2020'}), 'description':'RGB LED with integrated controller', '_match_pin_regex':False, 'datasheet':'http://www.led-color.com/upload/201604/APA102-2020%20SMD%20LED.pdf', 'keywords':'RGB LED addressable 8bit pwm 5bit greyscale', '_name':'APA102-2020', 'ref_prefix':'D', 'num_units':None, 'fplist':['LED_SMD:LED-APA102-2020'], 'do_erc':True, 'aliases':Alias({'APA102-2020'}), 'pin':None, 'footprint':'tiny_pov_footprints.pretty:LED-APA102-2020', 'pins':[
            Pin(num='1',name='VDD',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='2',name='CKO',func=Pin.types.OUTPUT,do_erc=True),
            Pin(num='3',name='SDO',func=Pin.types.OUTPUT,do_erc=True),
            Pin(num='4',name='SDI',func=Pin.types.INPUT,do_erc=True),
            Pin(num='5',name='CKI',func=Pin.types.INPUT,do_erc=True),
            Pin(num='6',name='GND',func=Pin.types.PWRIN,do_erc=True)] }),
        Part(**{ 'name':'C', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'C'}), 'description':'Unpolarized capacitor', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'cap capacitor', '_name':'C', 'ref_prefix':'C', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'C'}), 'pin':None, 'footprint':None, 'pins':[
            Pin(num='1',name='~',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='~',func=Pin.types.PASSIVE,do_erc=True)] }),
        Part(**{ 'name':'AH1806W', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'AH1806W'}), 'description':'High Sensitivity Micropower Omnipolar Hall-Effect Switch, SC-59', '_match_pin_regex':False, 'datasheet':'https://www.diodes.com/assets/Datasheets/AH1806.pdf', 'keywords':'hall switch', '_name':'AH1806W', 'ref_prefix':'U', 'num_units':None, 'fplist':['Package_TO_SOT_SMD:SC-59'], 'do_erc':True, 'aliases':Alias({'AH1806W'}), 'pin':None, 'footprint':'tiny_pov_footprints.pretty:SC-59_Handsoldering', 'pins':[
            Pin(num='1',name='VDD',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='2',name='OUTPUT',func=Pin.types.OPENCOLL,do_erc=True),
            Pin(num='3',name='GND',func=Pin.types.PWRIN,do_erc=True)] }),
        Part(**{ 'name':'STM32G030J6Mx', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'STM32G030J6Mx'}), 'description':'STMicroelectronics Arm Cortex-M0+ MCU, 32KB flash, 8KB RAM, 64 MHz, 2.0-3.6V, 6 GPIO, SO8N', '_match_pin_regex':False, 'datasheet':'https://www.st.com/resource/en/datasheet/stm32g030j6.pdf', 'keywords':'Arm Cortex-M0+ STM32G0 STM32G0x0 Value line', '_name':'STM32G030J6Mx', 'ref_prefix':'U', 'num_units':None, 'fplist':['Package_SO:SOIC-8_3.9x4.9mm_P1.27mm'], 'do_erc':True, 'aliases':Alias({'STM32G030J6Mx'}), 'pin':None, 'footprint':'tiny_pov_footprints.pretty:STM32_SO_8N', 'pins':[
            Pin(num='1',name='PC14/PB7/PB8/PB9',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='2',name='VDD',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='3',name='VSS',func=Pin.types.PWRIN,do_erc=True),
            Pin(num='4',name='NRST/PA0/PA1/PA2',func=Pin.types.INPUT,do_erc=True),
            Pin(num='5',name='PB0/PB1/PA8/PA11/PA9',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='6',name='PA12/PA10',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='7',name='PA13',func=Pin.types.BIDIR,do_erc=True),
            Pin(num='8',name='PA14/PA15/PB5/PB6',func=Pin.types.BIDIR,do_erc=True)] }),
        Part(**{ 'name':'SolderJumper_2_Open', 'dest':TEMPLATE, 'tool':SKIDL, '_aliases':Alias({'SolderJumper_2_Open'}), 'description':'Solder Jumper, 2-pole, open', '_match_pin_regex':False, 'datasheet':'~', 'keywords':'solder jumper SPST', '_name':'SolderJumper_2_Open', 'ref_prefix':'JP', 'num_units':None, 'fplist':[''], 'do_erc':True, 'aliases':Alias({'SolderJumper_2_Open'}), 'pin':None, 'footprint':'tiny_pov_footprints.pretty:SolderJumper', 'pins':[
            Pin(num='1',name='A',func=Pin.types.PASSIVE,do_erc=True),
            Pin(num='2',name='B',func=Pin.types.PASSIVE,do_erc=True)] })])