import pykraken as kn
import math as m

kn.init()
kn.window.create((200, 150), scaled=True)
clock = kn.Clock()

class Player:
    def __init__(self):
        self.pos = kn.Vec2(kn.window.get_size()) / 2
        self.rect = kn.Rect(0, 0, 10, 10)
        self.speed = 100
        
        self.curr_pad = 0
        
    def update(self, delta: float):
        if kn.key.is_just_pressed(kn.S_SPACE):
            self.curr_pad = 1 if self.curr_pad == 0 else 0
        dir_vec = kn.gamepad.get_left_stick(self.curr_pad)
        
        self.pos += dir_vec * self.speed * delta
        self.rect.center = self.pos
        
        kn.draw.rect(self.rect, (255, 255, 255))
        
        kn.draw.line(self.pos, kn.mouse.get_pos(), (255, 0, 0), 1)

player = Player()
color = kn.Color("#141414")

while kn.window.is_open():
    delta = clock.tick(240)
    kn.event.poll()
    
    kn.window.clear(color)
    player.update(delta)
    
    kn.window.flip()

kn.quit()
