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
        
    def update(self, delta: float):
        dir_vec = kn.Vec2(
            kn.key.is_pressed(kn.S_d) - kn.key.is_pressed(kn.S_a),
            kn.key.is_pressed(kn.S_s) - kn.key.is_pressed(kn.S_w)
        )
        if dir_vec:
            dir_vec.normalize()
        
        self.pos += dir_vec * self.speed * delta
        self.rect.center = self.pos
        
        kn.draw.rect(self.rect, (255, 255, 255))

player = Player()
color = kn.Color("#141414")
p = kn.Vec2(kn.window.get_size()) / 2


def draw_circle(pos, radius):
    if radius < 1:
        radius = 1
        
    x = 0
    y = radius
    d = 3 - (2 * radius)
    
    kn.draw.point(pos + (x, -y), (255, 255, 255))
    kn.draw.point(pos + (x, y), (255, 255, 255))
    kn.draw.point(pos + (-x, -y), (255, 255, 255))
    kn.draw.point(pos + (-x, y), (255, 255, 255))
    kn.draw.point(pos + (y, -x), (255, 255, 255))
    kn.draw.point(pos + (y, x), (255, 255, 255))
    kn.draw.point(pos + (-y, -x), (255, 255, 255))
    kn.draw.point(pos + (-y, x), (255, 255, 255))
    
    last_y = 0
    while x <= y:
        if x == y and y == last_y:
            x += 1
            continue
        last_y = y
        
        if d < 0:
            d += 4 * x + 6
        else:
            d += 4 * (x - y) + 10
            y -= 1
        x += 1
        
        if radius == 4 and x == y:
            continue
        
        kn.draw.point(pos + (x, -y), (255, 255, 255))
        kn.draw.point(pos + (x, y), (255, 255, 255))
        kn.draw.point(pos + (-x, -y), (255, 255, 255))
        kn.draw.point(pos + (-x, y), (255, 255, 255))
        kn.draw.point(pos + (y, -x), (255, 255, 255))
        kn.draw.point(pos + (y, x), (255, 255, 255))
        kn.draw.point(pos + (-y, -x), (255, 255, 255))
        kn.draw.point(pos + (-y, x), (255, 255, 255))

radius = 1
while kn.window.is_open():
    delta = clock.tick(240)
    kn.event.poll()
    
    kn.window.clear(color)
    # player.update(delta)
    
    if kn.key.is_just_pressed(kn.S_d):
        radius += 1
        print(radius)
    if kn.key.is_just_pressed(kn.S_a):
        radius -= 1
        print(radius)
        
    draw_circle(p, radius)
    # kn.draw.circle(p, radius, (255, 255, 255))
    
    kn.window.flip()

kn.quit()
