import pcbnew

board = pcbnew.GetBoard()

fp_list = board.GetFootprints()

def get_point_mm(x, y):
    return pcbnew.VECTOR2I(pcbnew.wxPoint(x * 10e5, y * 10e5))

def layout_leds(count, spacing, offset_x = 0, offset_y = 0):
    for i in range(count):
        led = board.FindFootprintByReference(f'D{i+1}')
        x = -(i * spacing - count * spacing * 0.5 + 1)
        led.SetPosition(get_point_mm(x + offset_x, offset_y))
    for g in board.Groups():
        if g.GetName() == 'arm_outline':
            g.SetPosition(get_point_mm(offset_x, offset_y))
