import pykraken as kn

kn.init()
kn.window.create((200, 150), scaled=True)
clock = kn.Clock()

rect = kn.Rect()
rect.size = (2, 2)
rect.size = kn.Vec2(2, 2)
rect.size += kn.Vec2(2, 2)

color = kn.Color("#141414")

while kn.window.is_open():
    clock.tick(60)
    kn.event.poll()
    kn.window.clear(color)
    kn.window.flip()

kn.quit()
