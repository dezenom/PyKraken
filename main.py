import pykraken as kn

kn.init()
kn.window.create((200, 150), scaled=True)
clock = kn.Clock()

class Player:
    def __init__(self, pad):
        self.pos = kn.Vec2(kn.window.get_size()) / 2
        self.rect = kn.Rect(0, 0, 10, 10)
        self.speed = 100
        
        self.curr_pad = pad
        
    def update(self, delta: float):
        # dir_vec = kn.gamepad.get_left_stick(self.curr_pad)
        dir_vec = kn.input.get_direction("up", "right", "down", "left")
        
        self.pos += dir_vec * self.speed * delta
        self.rect.center = self.pos
        
        kn.draw.rect(self.rect, (255, 255, 255))
        
        kn.draw.line(self.pos, kn.mouse.get_pos(), (255, 0, 0), 1)

playerA = Player(0)
# playerB = Player(1)
color = kn.Color("#141414")

kn.input.bind(
    "up",
    [
        kn.InputAction(kn.S_w),
        kn.InputAction(kn.S_UP),
        kn.InputAction(kn.C_LY, False, 0),
    ]
)
kn.input.bind(
    "right",
    [
        kn.InputAction(kn.S_d),
        kn.InputAction(kn.S_RIGHT),
        kn.InputAction(kn.C_LX, True, 0),
    ]
)
kn.input.bind(
    "down",
    [
        kn.InputAction(kn.S_s),
        kn.InputAction(kn.S_DOWN),
        kn.InputAction(kn.C_LY, True, 0),
    ]
)
kn.input.bind(
    "left",
    [
        kn.InputAction(kn.S_a),
        kn.InputAction(kn.S_LEFT),
        kn.InputAction(kn.C_LX, False, 0),
    ]
)

while kn.window.is_open():
    delta = clock.tick(240)
    kn.event.poll()
    
    kn.window.clear(color)
    playerA.update(delta)
    # playerB.update(delta)
    
    kn.window.flip()

kn.quit()
