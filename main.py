import pykraken as kn

kn.init()
kn.window.create((200, 150), scaled=True)
clock = kn.Clock()

rect = kn.Rect()

while kn.window.is_open():
    clock.tick(60)
    kn.event.poll()
    kn.window.clear((20, 20, 20))
    kn.window.flip()

kn.quit()
